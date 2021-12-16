/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 22:37:23 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/09/27 05:13:54 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ppx_close(t_ppx *env, int fd)
{
	if (close(fd) == ERROR)
		ppx_exit_with_error_message(env, 5);
}

void	ppx_dup2(t_ppx *env, int fd1, int fd2)
{
	if (dup2(fd1, fd2) == ERROR)
		ppx_exit_with_error_message(env, 4);
}

void	ppx_pipe(t_ppx *env, int *fds)
{
	if (pipe(fds) == ERROR)
		ppx_exit_with_error_message(env, 3);
}

pid_t	ppx_fork(t_ppx *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		ppx_exit_with_error_message(env, 2);
	return (pid);
}

void	*ppx_malloc(t_ppx *env, size_t num, size_t size)
{
	void	*ptr;

	ptr = (void *)malloc(num * size);
	if (!ptr)
		ppx_exit_with_error_message(env, 1);
	return (ptr);
}
