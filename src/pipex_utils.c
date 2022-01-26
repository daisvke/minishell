/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 04:00:34 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/25 09:38:52 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_save_data_from_child(t_ms *ms_env, t_ppx *ppx_env)
{
	if (ppx_env->i > 0)
		ms_close(ms_env, ppx_env->pipe_fds[ppx_env->i - 1][0]);
	ms_close(ms_env, ppx_env->pipe_fds[ppx_env->i][1]);
	ppx_env->fd_in = ppx_env->pipe_fds[ppx_env->i][0];
}

int	ppx_create_array_of_commands(t_ms *ms_env, t_ppx *ppx_env, char *cmd_line[])
{
	ppx_env->cmd = ppx_split(cmd_line[ppx_env->pos], ' ');
	if (!ppx_env->cmd)
		ms_exit_with_error_message(ms_env, 7);
	if (!*ppx_env->cmd)
		return (MS_READ_NONE);
	return (MS_SUCCESS);
}

bool	ppx_pipe_is_off_and_cmd_is_implemented(t_ppx *env, size_t *cmd_code)
{
	return ((env->options & MS_OPT_PIPE) == false \
		&& ms_check_if_the_cmd_is_implemented(\
			env->cmd, cmd_code, PPX_PROC_PARENT) == true);
}
