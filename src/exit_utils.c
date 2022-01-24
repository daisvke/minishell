/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 02:52:55 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/24 02:53:29 by dtanigaw         ###   ########.fr       */
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
	exit(EXIT_SUCCESS);
}
