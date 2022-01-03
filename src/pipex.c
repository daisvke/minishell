/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/02 20:26:05 by dtanigaw         ###   ########.fr       */
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
			return (MS_ERROR);
		++i;
	}
	return (MS_OK);
}

void	ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *path)
{
	size_t	i;
	size_t	len;
	char	*current_absolute_path;

	if (ms_check_if_there_is_not_too_much_args(ppx_env->cmd) == MS_OK)
	{
		chdir(path);
		current_absolute_path = getcwd(NULL, 0);
		if (current_absolute_path == NULL)
			ppx_exit_with_error_message(ppx_env, 10);
		i = ppx_get_key_value_from_envp(ms_env->envp, "PWD=").index;
		free(ms_env->envp[i]);
		ms_env->envp[i] = ppx_join_three_str(ppx_env, "PWD", "=", current_absolute_path);
	}
	else
		write(STDOUT_FILENO, "cd: Too many arguments\n", 24);
}

void	ms_execute_cmd_pwd(char *envp[])
{
	char	*current_path;

	current_path = ppx_get_key_value_from_envp(envp, "PWD=").value;
	printf("%s\n", current_path);
}

void	ms_execute_cmd_env(t_env_lst *envp_head)
{
	t_env_lst *node;

	node = envp_head;
	while (node)
	{
		printf("%s\n", node->entry);
		node = node->next;
	}
}
bool	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

bool	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	ms_compare_with_envp_key(const char *envp_entry, const char *str)
{
	if (str && envp_entry)
	{
		while (*str && *envp_entry && *envp_entry != '=')
		{
			if (*str != *envp_entry)
				return ((unsigned char)(*str) - (unsigned char)(*envp_entry));
			str++;
			envp_entry++;
		}
		if (*str == '=')
			return (MS_SAME);
	}
	return (DIFFERENT);
}
/*
void	ms_delete_entry_from_envp(t_ms *env, char *new_entry, size_t index)
{
	free(env->envp[index]);
	env->envp[index] = ppx_strdup;
}

int	ms_get_key_length_from_entry(char *entry)
{
	size_t	i;

	i = 0;
	while (entry[i])
	{
		if (entry[i] == '=')
		{
			if (i == 0 && entry[i + 1] == '=')
				return (-2);
			if (entry[i + 1] == '\0')
				return (-1);
			return (i);
		}
		++i;
	}
	return (-1);
}

void	ms_execute_unset(t_ms *env, char *cmd)
{
	size_t	i;
	size_t	j;
	size_t	len;

	i = 1;
	while (cmd[i])
	{
		len = ms_get_key_length_from_entry(cmd[i]);
		if (len == -1)
			return ;
		else if (len == -2)
		{
			printf("minishell: unset: `%s' : not a valid identifier\n", cmd[i]);
			return ;
		}
		j = 0;
		while (env->envp[j])
		{
			if (ms_cmp_str_with_envp_entry(cmd[i], env->envp[j], len) == MS_SAME)
				ms_delete_entry_from_envp(ms_env, cmd[i], j);
			++j;
		}
		++i
	}
}
*/
int	ms_check_export_args(t_ms *env, char *cmd[])
{
	size_t	i;
	size_t	j;

	i = MS_FIRST_ARG_POS;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (ft_isalnum(cmd[i][j]) == false)
			{
				printf("minishell: export: `%s' : not a valid identifier\n", cmd[i]);
				return (MS_ERROR);
			}
			++j;
		}
		if (cmd[i][j] == '=')
		{
			if (j == 0 || cmd[i][j - 1] == ' ')
			{
				printf("minishell: export: `%s' : not a valid identifier\n", cmd[i]);
				return (MS_ERROR);
			}
		}
		else
			return (MS_NO_EXPORT);
		++i;
	}
	return (0);
}

void    ms_execute_cmd_export(t_ms *env, char *cmd_line[])
{
    size_t  i;
	size_t	j;
    size_t  len;
	t_env_lst   *node;
	t_env_lst   *new;

	if (ms_check_export_args(env, cmd_line) == MS_NO_EXPORT \
		|| ms_check_export_args(env, cmd_line) == MS_ERROR)
		return ;
	i = MS_FIRST_ARG_POS;
	while (cmd_line[i])
	{
        node = env->envp_lst;
		while (node && ms_compare_with_envp_key(node->entry, cmd_line[i]) != MS_SAME)
			node = node->next;
		new = ms_lst_create_new_node(cmd_line[i]);
		if (cmd_line[i])
			free(cmd_line[i]);
        if (node->next == NULL)
            ms_lst_add_back(env->envp_lst, new);
        else
        {
            free(node->entry);
            len = ppx_strlen(cmd_line[i]);
            node->entry = ms_strdup(cmd_line[i], len);
        }
		++i;
	}
	env->envp[j] = malloc(sizeof(char));
	env->envp[j] = 0;
}

void	ms_execute_cmd_echo(char *cmd[])
{
	bool	opt_n;	
	size_t	i;

	opt_n = false;
	if (ms_strcmp(cmd[1], "-n") == MS_SAME)
		opt_n = true;
	i = MS_FIRST_ARG_POS + opt_n;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1] != NULL)
			printf(" ");
		++i;
	}
	if (opt_n == false)
		printf("\n");
}

void	ppx_execute_implemented_cmd_in_child(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[])
{
	if (cmd_code == MS_CMD_ECHO)
		ms_execute_cmd_echo(ppx_env->cmd);
	else if (cmd_code == MS_CMD_PWD)
		ms_execute_cmd_pwd(ms_env->envp);
	else if (cmd_code == MS_CMD_ENV)
		ms_execute_cmd_env(ms_env->envp_lst);
}

void	ppx_execute_implemented_cmd_in_parent(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[])
{
	if (cmd_code == MS_CMD_CD)
		ms_execute_cmd_cd(ms_env, ppx_env, cmd[1]);
	else if (cmd_code == MS_CMD_EXPORT)
		ms_execute_cmd_export(ms_env, cmd);
//	else if (cmd_code == MS_CMD_UNSET)
//		ms_execute_cmd_unset(ms_env, cmd);
	else if (cmd_code == MS_CMD_EXIT)
		exit(EXIT_SUCCESS);
}

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env, char *argv[], char *envp[])
{
	size_t	cmd_code;
	char	*path_to_cmd;

	if (ppx_env->options & MS_OPT_PIPE \
		|| (ppx_env->options & MS_OPT_REDIR_OUTPUT))
	{
		ppx_close(ppx_env, ppx_env->pipe_fds[ppx_env->i][0]);
		ppx_dup2(ppx_env, ppx_env->fd_in, STDIN_FILENO);
		if (ppx_env->pos != ppx_env->argc - 1)
	    	ppx_dup2(ppx_env, ppx_env->pipe_fds[ppx_env->i][1], STDOUT_FILENO);
		ppx_get_fd(ppx_env, argv);
	}
	// if not implemented
		// check if implm
	if (ms_check_if_the_cmd_is_implemented(ppx_env, ppx_env->cmd, &cmd_code, PPX_PROC_PARENT) == true)
		ppx_execute_implemented_cmd_in_parent(ms_env, ppx_env, cmd_code, ppx_env->cmd);
	else if (ms_check_if_the_cmd_is_implemented(ppx_env, ppx_env->cmd, &cmd_code, PPX_PROC_CHILD) == true)
		ppx_execute_implemented_cmd_in_child(ms_env, ppx_env, cmd_code, ppx_env->cmd);
	else
	{

		//if (ms_strcmp(ppx_env->cmd[0], "ls") == MS_SAME)
		//	ms_add_curr_path_to_ls_cmd(ppx_env, ppx_env->cmd);
		path_to_cmd = ppx_get_the_right_cmd_path(ppx_env, envp, "PATH=", ppx_env->cmd[0]);
		if (execve(path_to_cmd, ppx_env->cmd, envp) == ERROR)
			ppx_exit_when_cmd_not_found(ppx_env, ppx_env->cmd[0]);
	}
}

void	ppx_save_data_from_child(t_ppx *env)
{
	if (env->i > 0)
		ppx_close(env, env->pipe_fds[env->i - 1][0]);
	ppx_close(env, env->pipe_fds[env->i][1]);
	env->fd_in = env->pipe_fds[env->i][0];
}

int	ppx_wait_for_all_children(t_ms *ms_env, t_ppx *ppx_env, pid_t pid)
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
		ms_env->last_pipe_exit_status = wstatus;
		if (WIFEXITED(wstatus))
		{
			status_code = WEXITSTATUS(wstatus);
			if (status_code != 0)
				return (status_code);
		}
		++i;
	}
	return (0);
}

int	ppx_pipex(char *argv[], char *envp[], t_ppx *ppx_env, t_ms *ms_env)
{
	pid_t	pid;
	int		err;
	size_t	cmd_code;

	if (ppx_env->options & MS_OPT_REDIR_OUTPUT)
		ppx_env->pos += GET_FIRST_CMD;
	if (ppx_env->options & MS_OPT_HEREDOC)
		ppx_input_heredoc(ppx_env, argv);
	while (ppx_env->pos < ppx_env->argc)
	{
		// gen cmd
		ppx_env->cmd = ppx_split(argv[ppx_env->pos], ' ');
		if (!ppx_env->cmd)
			ppx_exit_with_error_message(ppx_env, 7);
		if (!*ppx_env->cmd)
			return (0);
		add_history(ms_env->cmd_line);

		// if no pipe + builtin
		if ((ppx_env->options & MS_OPT_PIPE) == false \
			&& ms_check_if_the_cmd_is_implemented(ppx_env, ppx_env->cmd, &cmd_code, PPX_PROC_PARENT) == true)
			ppx_execute_implemented_cmd_in_parent(ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else
		{
		if (ppx_env->options & MS_OPT_PIPE)
		ppx_pipe(ppx_env, ppx_env->pipe_fds[ppx_env->i]);

		pid = ppx_fork(ppx_env);
		if (pid == CHILD)
		{
			ppx_spawn_child_to_execute_cmd(ms_env, ppx_env, argv, envp);
			exit(EXIT_SUCCESS);
		}
		if ((ppx_env->options & MS_OPT_PIPE)
			&& ppx_env->pos != ppx_env->argc - 1)
			ppx_save_data_from_child(ppx_env);
		}
		++ppx_env->pos;
		++ppx_env->i;
	}
	err = 0;
	if (!((ppx_env->options & MS_OPT_PIPE) == false \
		&& ms_check_if_the_cmd_is_implemented(ppx_env, ppx_env->cmd, &cmd_code, PPX_PROC_PARENT) == true))
		err = ppx_wait_for_all_children(ms_env, ppx_env, pid);
	if (ppx_env->options & MS_OPT_HEREDOC)
		unlink("heredoc_output");
	ppx_free_pipe_fds(ppx_env);
	return (err);
}
