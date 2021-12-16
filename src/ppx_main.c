/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:06 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/16 03:29:31 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	ppx_init_ppx(int argc, char *argv[], t_ppx *env)
{
	env->pos = 0;
	env->i = 0;
	env->argc = 0;
	env->fd_in = 0;
	env->argc = argc;
	env->cmd = NULL;
	env->heredoc = false;
	if (ppx_strncmp(argv[1], "here_doc", 8) == SAME)
	{
		env->heredoc = true;
		++env->pos;
	}
	env->cmd_nbr = env->argc - 3 - env->pos;
	ppx_init_pipe_fds(env);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ppx	env;
	int		res;

	env.cmd_nbr = 0;
	if (argc < 5)
		ppx_exit_with_error_message(&env, 0);
	ppx_init_ppx(argc, argv, &env);
	res = ppx_pipex(argv, envp, &env);
	exit(res);
}
