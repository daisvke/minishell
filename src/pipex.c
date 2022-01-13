/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/11 08:28:17 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_check_if_there_is_not_too_much_args(char **cmd_and_args)
{
	size_t	i;

	i = 0;
	while (cmd_and_args[i])
	{
		if (i > 1)
			return (MS_ERROR); //error
		++i;
	}
	return (MS_OK);
}

bool	ms_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

bool	ms_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool	ms_isalnum(int c)
{
	return (ms_isalpha(c) || ms_isdigit(c));
}

int	ms_compare_with_envp_key(const char *envp_entry, const char *str, bool equal_in_str)
{
	if (str && envp_entry)
	{
		while (*str && *envp_entry)
		{
			if (equal_in_str == true && *envp_entry == '=')
				break ;
			if (*str != *envp_entry)
				return ((unsigned char)(*str) - (unsigned char)(*envp_entry));
			str++;
			envp_entry++;
		}
		if (equal_in_str == true && *str == '=')
			return (MS_SAME);
		else if (equal_in_str == false && *envp_entry == '=')
			return (MS_SAME);
	}
	return (MS_DIFFERENT);
}

void	ppx_execute_implemented_cmd_in_parent(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[])
{
	if (cmd_code == MS_CMD_CD)
		ms_execute_cmd_cd(ms_env, ppx_env, cmd[1]);
	else if (cmd_code == MS_CMD_EXPORT)
		ms_execute_cmd_export(ms_env, cmd);
	else if (cmd_code == MS_CMD_UNSET)
		ms_execute_cmd_unset(ms_env, cmd);
	else if (cmd_code == MS_CMD_EXIT)
		exit(EXIT_SUCCESS);
}

void	ppx_execute_implemented_cmd(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[])
{
	if (cmd_code == MS_CMD_ECHO)
		ms_execute_cmd_echo(ppx_env->cmd);
	else if (cmd_code == MS_CMD_PWD)
		ms_execute_cmd_pwd(ms_env->envp_lst);
	else if (cmd_code == MS_CMD_ENV)
		ms_execute_cmd_env(ms_env->envp_lst);
	ppx_execute_implemented_cmd_in_parent(ms_env, ppx_env, cmd_code, cmd);
}

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	cmd_code;
	char	*path_to_cmd;
	size_t	lst_size;
	char	**envp;
	
	ppx_handle_redirections(ppx_env);
	if (ppx_env->cmd)
	{
		if (ppx_env->options & MS_OPT_PIPE)
		{
			ppx_close(ppx_env, ppx_env->pipe_fds[ppx_env->i][0]);
			ppx_dup2(ppx_env, ppx_env->fd_in, STDIN_FILENO);
			if (ppx_env->pos != ppx_env->cmd_nbr - 1 \
				&& (ms_env->options & MS_OPT_REDIR_OUTPUT) == false)
				ppx_dup2(ppx_env, ppx_env->pipe_fds[ppx_env->i][1], STDOUT_FILENO);
	//		ppx_get_fd(ppx_env, argv);
		}
		if (ms_check_if_the_cmd_is_implemented(ppx_env->cmd, &cmd_code, PPX_PROC_CHILD) == true)
			ppx_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else
		{
			path_to_cmd = ppx_get_the_right_cmd_path(ms_env, ppx_env, "PATH=", ppx_env->cmd[0]);
			lst_size = ms_lst_lstsize(ms_env->envp_lst);
			envp = ms_convert_envp_lst_to_array_of_pointers(ms_env->envp_lst, lst_size);
			if (execve(path_to_cmd, ppx_env->cmd, envp) == PPX_ERROR)
				ppx_exit_when_cmd_not_found(ppx_env, ppx_env->cmd[0]);
			//no exit+error handling 
			ppx_free_array_of_pointers(envp, lst_size);
		}
	}
}


void	ppx_save_data_from_child(t_ppx *env)
{
	if (env->i > 0)
		ppx_close(env, env->pipe_fds[env->i - 1][0]);
	ppx_close(env, env->pipe_fds[env->i][1]);
	env->fd_in = env->pipe_fds[env->i][0];
}

void	ppx_wait_for_all_children(t_ms *ms_env, t_ppx *ppx_env, pid_t pid)
{
	int	i;
	int	size;
	int	wstatus;
	int	status_code;

	i = 0;
	size = ppx_env->cmd_nbr;
	while (i < size)
	{
		waitpid(pid, &wstatus, WUNTRACED);
		if (WIFEXITED(wstatus))
		{
			status_code = WEXITSTATUS(wstatus);
			ms_env->last_pipe_exit_status = status_code;
			return ;
		}
		++i;
	}
	return ;
}

int	ppx_generate_array_of_commands(t_ms *ms_env, t_ppx *ppx_env, char *cmd_line[])
{
	ppx_env->cmd = ppx_split(cmd_line[ppx_env->pos], ' ');
	if (!ppx_env->cmd)
		ms_exit_with_error_message(ppx_env, 7);
	if (!*ppx_env->cmd)
		return (MS_READ_NONE);
	return (MS_SUCCESS);
}

void	ppx_execute_pipe_and_run_cmd_in_child_process(t_ms *ms_env, t_ppx *ppx_env, char *cmd_line[], pid_t *pid)
{
	if (ppx_env->options & MS_OPT_PIPE)
		ppx_pipe(ppx_env, ppx_env->pipe_fds[ppx_env->i]);
	*pid = ppx_fork(ppx_env);
	if (*pid == PPX_PROC_CHILD)
	{
		ppx_spawn_child_to_execute_cmd(ms_env, ppx_env);
		exit(EXIT_SUCCESS);
	}
	if ((ppx_env->options & MS_OPT_PIPE)
		&& ppx_env->pos != ppx_env->cmd_nbr - 1)
		ppx_save_data_from_child(ppx_env);
}

bool	ppx_pipe_is_off_and_cmd_is_implemented(t_ppx *env, size_t *cmd_code)
{
	return ((env->options & MS_OPT_PIPE) == false \
		&& ms_check_if_the_cmd_is_implemented(env->cmd, &cmd_code, PPX_PROC_PARENT) == true);
}

void	ppx_pipex(t_ms *ms_env, t_ppx *ppx_env, char *cmd_line[])
{
	pid_t	pid;
	size_t	cmd_code;

	while (ppx_env->pos < ppx_env->cmd_nbr)
	{
		if (ppx_generate_array_of_commands(ms_env, ppx_env, cmd_line) == MS_READ_NONE)
			return ;
		add_history(ms_env->cmd_line);
		if (ppx_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == true)
			ppx_execute_implemented_cmd_in_parent(ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else
			ppx_execute_pipe_and_run_cmd_in_child_process(ms_env, ppx_env, cmd_line, &pid);
		++ppx_env->pos;
		++ppx_env->i;
	}
	if (ppx_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == false)
		ppx_wait_for_all_children(ms_env, ppx_env, pid);
	ppx_free_pipe_fds(ppx_env);
}
