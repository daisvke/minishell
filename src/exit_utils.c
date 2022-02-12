/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 02:52:55 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/12 10:51:13 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_free_pipe_fds(t_ppx *env)
{
	int	size;
	int	i;

	if (env->pipe_fds == NULL)
		return ;
	size = env->cmd_nbr;
	i = 0;
	while (i < size)
	{
		env->pipe_fds[i] = ms_free(env->pipe_fds[i]);
		++i;
	}
	env->pipe_fds = ms_free(env->pipe_fds);
}

void	ms_quit_with_ctrl_d(t_ms *env)
{
	rl_clear_history();
	ms_lst_clear_list(env->envp_lst);
	ms_free_prompt_struct(env);
	exit(EXIT_SUCCESS);
}

void	ms_close_pipe_fds(t_ms *ms_env, t_ppx *ppx_env, bool after_increm)
{
	struct stat	statbuf;
	size_t		i;
	size_t		max;

	if (ppx_env->options & MS_OPT_PIPE)
	{
		max = ppx_env->i;
		if (after_increm)
			max -= 1;
		if (max < 0)
			max = 0;
		i = 0;
		while (i <= max)
		{
			if (fstat(ppx_env->pipe_fds[i][0], &statbuf) == MS_SUCCESS)
				ms_close(ms_env, ppx_env->pipe_fds[i][0]);
			if (fstat(ppx_env->pipe_fds[i][1], &statbuf) == MS_SUCCESS)
				ms_close(ms_env, ppx_env->pipe_fds[i][1]);
			++i;
		}
	}
}

void	ppx_close_pipe_fds(t_ppx *env)
{
	struct stat	statbuf;
	size_t		i;

	if (env->options & MS_OPT_PIPE)
	{
		i = 0;
		while (i <= env->i)
		{
			if (fstat(env->pipe_fds[i][0], &statbuf) == MS_SUCCESS)
				ppx_close(env, env->pipe_fds[i][0]);
			if (fstat(env->pipe_fds[i][1], &statbuf) == MS_SUCCESS)
				ppx_close(env, env->pipe_fds[i][1]);
			++i;
		}
	}
}
