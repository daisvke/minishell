/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls_unistd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 04:19:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 00:30:02 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_close(t_ms *env, int fd)
{
	if (close(fd) == PPX_ERROR)
		ms_exit_with_error_message(env, 8);
}

void	ms_dup2(t_ms *env, int fd1, int fd2, bool close_fd)
{
	if (dup2(fd1, fd2) == PPX_ERROR)
		ms_exit_with_error_message(env, 3);
	if (close_fd == true)
		ms_close(env, fd1);
}

pid_t	ms_fork(t_ms *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == PPX_ERROR)
		ms_exit_with_error_message(env, 7);
	return (pid);
}

void	ms_pipe(t_ms *env, int *fds)
{
	if (pipe(fds) == PPX_ERROR)
		ms_exit_with_error_message(env, 10);
}
