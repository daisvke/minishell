/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:06 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/28 05:30:58 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_init_pipe_fds(t_ppx *env)
{
	int	size;
	int	i;

	size = env->cmd_nbr;
	env->pipe_fds = malloc(sizeof(int *) * size);
	i = 0;
	while (i < size)
	{
		env->pipe_fds[i] = malloc(sizeof(int) * 2);
		env->pipe_fds[i][0] = 0;
		env->pipe_fds[i][1] = 0;
		++i;
	}
}

void	ppx_init_ppx(int argc, t_ppx *ppx_env, t_ms *ms_env)
{
	ms_memset(ppx_env, 0, sizeof(t_ppx));
	ppx_env->options = ms_env->options;
	ppx_env->cmd_nbr = argc;
	ppx_init_pipe_fds(ppx_env);
}

int	ppx_main(int argc, char *argv[], t_ms *ms_env)
{
	t_ppx	ppx_env;
	int		res;

	ppx_init_ppx(argc, &ppx_env, ms_env);
	res = ppx_pipex(argv, &ppx_env, ms_env);
	return (SUCCESS);
}
