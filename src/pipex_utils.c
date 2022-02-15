/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 04:00:34 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/16 00:40:18 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_wait_for_all_children(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t pid, size_t wait_count)
{
	int	i;
	int	size;
	int	wstatus;
	int	status_code;

	i = 0;
	size = ppx_env->cmd_nbr - wait_count;
	while (i < size)
	{
		wstatus = 0;
		if (waitpid(pid, &wstatus, WUNTRACED) == PPX_ERROR)
		{
			ppx_free_all_allocated_variables(&ms_env->ppx_env);
			ppx_free_array_of_pointers(&ms_env->split_cmdline, MS_ALL);
		}
		if (WIFEXITED(wstatus))
		{
			status_code = WEXITSTATUS(wstatus);
			ms_env->last_pipe_exit_status = status_code;
			return ;
		}
		++i;
	}
	return ;
}

void	ppx_wait_for_proc_with_heredoc(\
	t_ms *ms_env, pid_t pid, size_t *wait_count)
{
	int	wstatus;
	int	status_code;

	wstatus = 0;
	waitpid(pid, &wstatus, WUNTRACED);
	if (WIFEXITED(wstatus))
	{
		status_code = WEXITSTATUS(wstatus);
		ms_env->last_pipe_exit_status = status_code;
	}
	else
		*wait_count += 1;
}

int	ppx_create_array_of_commands(t_ms *ms_env, t_ppx *ppx_env, char *cmdline[])
{
	ppx_env->cmd = ppx_split(cmdline[ppx_env->i], ' ');
	if (!ppx_env->cmd)
		ms_exit_with_error_message(ms_env, 7);
	if (!*ppx_env->cmd || !**ppx_env->cmd)
		return (MS_READ_NONE);
	return (MS_SUCCESS);
}

bool	ppx_pipe_is_off_and_cmd_is_implemented(t_ppx *env, size_t *cmd_code)
{
	return ((env->options & MS_OPT_PIPE) == false \
		&& ms_check_if_the_cmd_is_implemented(\
			env->cmd, cmd_code, PPX_PROC_PARENT) == true);
}
