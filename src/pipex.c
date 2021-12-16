/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/10/04 01:12:09 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ppx_spawn_child_to_execute_cmd(t_ppx *env, char *argv[], char *envp[])
{
	char	*path_to_cmd;
	int		fd;

	ppx_close(env, env->pipe_fds[env->i][0]);
	ppx_dup2(env, env->fd_in, STDIN_FILENO);
	ppx_dup2(env, env->pipe_fds[env->i][1], STDOUT_FILENO);
	fd = ppx_get_fd(env, argv);
	env->cmd = ppx_split(argv[env->pos], ' ');
	if (!env->cmd)
		ppx_exit_with_error_message(env, 7);
	path_to_cmd = ppx_get_the_right_cmd_path(env, envp, "PATH=", env->cmd[0]);
	if (execve(path_to_cmd, env->cmd, envp) == ERROR)
		ppx_exit_when_cmd_not_found(env, env->cmd[0]);
}

void	ppx_save_data_from_child(t_ppx *env)
{
	if (env->i > 0)
		ppx_close(env, env->pipe_fds[env->i - 1][0]);
	ppx_close(env, env->pipe_fds[env->i][1]);
	env->fd_in = env->pipe_fds[env->i][0];
	++env->pos;
	++env->i;
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

int	ppx_pipex(char *argv[], char *envp[], t_ppx *env)
{
	pid_t	pid;
	int		err;

	env->pos += GET_FIRST_CMD;
	if (env->heredoc)
		ppx_input_heredoc(env, argv);
	while (env->pos < env->argc - 1)
	{
		ppx_pipe(env, env->pipe_fds[env->i]);
		pid = ppx_fork(env);
		if (pid == CHILD)
		{
			ppx_spawn_child_to_execute_cmd(env, argv, envp);
			exit(EXIT_SUCCESS);
		}
		ppx_save_data_from_child(env);
	}
	err = ppx_wait_for_all_children(env, pid);
	if (env->heredoc)
		unlink("heredoc_output");
	ppx_free_pipe_fds(env);
	return (err);
}
