/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_init_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:06 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 08:32:14 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_init_pipe_fds(t_ms *ms_env, t_ppx *ppx_env)
{
	int	size;
	int	i;

	size = ppx_env->cmd_nbr;
	ppx_env->pipe_fds = ms_malloc(ms_env, size, sizeof(int *));
	i = 0;
	while (i < size)
	{
		ppx_env->pipe_fds[i] = ms_malloc(ms_env, 2, sizeof(int));
		ppx_env->pipe_fds[i][0] = 0;
		ppx_env->pipe_fds[i][1] = 0;
		++i;
	}
}

void	ppx_init_ppx(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_and_file_nbr)
{
	ms_memset(ppx_env, 0, sizeof(t_ppx));
	ppx_env->options = ms_env->options;
	ppx_env->cmd_nbr = cmd_and_file_nbr;
	ppx_init_pipe_fds(ms_env, ppx_env);
}
