/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 11:47:20 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *path)
{
	char	*current_absolute_path;

	chdir(path);
	current_absolute_path = getcwd(NULL, 0);
	if (current_absolute_path == NULL)
		ppx_exit_with_error_message(ppx_env, 10);
	//ms_free
	free(ms_env->current_directory);
	ms_env->current_directory = current_absolute_path;
}

void	ppx_execute_implemented_cmd(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *arg)
{
	if (cmd_code == MS_CMD_CD)
		ms_execute_cmd_cd(ms_env, ppx_env, arg);
	if (cmd_code == MS_CMD_PWD)	
		printf("%s\n", ms_env->current_directory);
}

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env, char *argv[], char *envp[])
{
	char	*path_to_cmd;

	if (ppx_env->pipe == true || ppx_env->redir_output == true)
	{
		ppx_close(ppx_env, ppx_env->pipe_fds[ppx_env->i][0]);
		ppx_dup2(ppx_env, ppx_env->fd_in, STDIN_FILENO);
		if (ppx_env->pos != ppx_env->argc - 1)
	    	ppx_dup2(ppx_env, ppx_env->pipe_fds[ppx_env->i][1], STDOUT_FILENO);
		ppx_get_fd(ppx_env, argv);
	}
	// if not implemented
//	if (ms_check_if_the_cmd_is_implemented(ppx_env, ppx_env->cmd, &cmd_code) == true)
//		ppx_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd[1]);
	else
	{
		if (ms_strcmp(ppx_env->cmd[0], "ls") == MS_SAME)
			ms_add_curr_path_to_ls_cmd(ppx_env, ppx_env->cmd);
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

int	ppx_wait_for_all_children(t_ppx *env, pid_t pid)
{
	int	i;
	int	size;
	int	wstatus;
	int	status_code;

	i = 0;
	size = env->cmd_nbr;
	while (i < size)
	{
		waitpid(pid, &wstatus, WUNTRACED);
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

	if (ppx_env->redir_output == true)
		ppx_env->pos += GET_FIRST_CMD;
	if (ppx_env->heredoc)
		ppx_input_heredoc(ppx_env, argv);
//	printf("argc: %d\n", ppx_env->argc);
	while (ppx_env->pos < ppx_env->argc)
	{
//	printf("pos: %d\n", ppx_env->pos);
		if (ppx_env->pipe == true)
			ppx_pipe(ppx_env, ppx_env->pipe_fds[ppx_env->i]);

		// gen cmd
		ppx_env->cmd = ppx_split(argv[ppx_env->pos], ' ');
		if (!ppx_env->cmd)
			ppx_exit_with_error_message(ppx_env, 7);

		// check if implm
		if (ms_check_if_the_cmd_is_implemented(ppx_env, ppx_env->cmd, &cmd_code) == true)
			ppx_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd[1]);
		else
		{
		pid = ppx_fork(ppx_env);
		if (pid == CHILD)
		{
			ppx_spawn_child_to_execute_cmd(ms_env, ppx_env, argv, envp);
			exit(EXIT_SUCCESS);
		}
		if (ppx_env->pipe == true && ppx_env->pos != ppx_env->argc - 1)
			ppx_save_data_from_child(ppx_env);
		}
		++ppx_env->pos;
		++ppx_env->i;
	}
	err = ppx_wait_for_all_children(ppx_env, pid);
	if (ppx_env->heredoc)
		unlink("heredoc_output");
	ppx_free_pipe_fds(ppx_env);
	return (err);
}
