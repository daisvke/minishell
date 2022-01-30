/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls_pipex.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 04:13:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/30 14:40:26 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_close(t_ppx *env, int fd)
{
	if (close(fd) == PPX_ERROR)
		ppx_exit_with_error_message(env, 5);
}

void	ppx_dup2(t_ppx *env, int fd1, int fd2, bool close_fd)
{
	if (dup2(fd1, fd2) == PPX_ERROR)
		ppx_exit_with_error_message(env, 4);
	if (close_fd == true)
		ppx_close(env, fd1);
}

pid_t	ppx_fork(t_ms *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == PPX_ERROR)
		ms_exit_with_error_message(env, 7);
	return (pid);
}

void	ppx_pipe(t_ms *env, int *fds)
{
	if (pipe(fds) == PPX_ERROR)
		ms_exit_with_error_message(env, 10);
}
