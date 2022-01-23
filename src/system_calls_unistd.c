/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls_unistd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 04:19:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/23 20:48:27 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_close(t_ms *env, int fd)
{
	if (close(fd) == PPX_ERROR)
		ms_exit_with_error_message(env, 8);
}

void	ms_dup2(t_ms *env, int fd1, int fd2)
{
	if (dup2(fd1, fd2) == PPX_ERROR)
		ms_exit_with_error_message(env, 3);
}
