/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_waitpid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 13:45:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/19 13:46:32 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_assign_exit_status_when_exited_by_signal(int wstatus)
{
	g_exit_status = ((1 << 7) | WTERMSIG(wstatus));
	if (g_exit_status == 130)
		write(STDOUT_FILENO, "\n", 1);
	else if (g_exit_status == 131)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void	ppx_wait_for_all_children(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t pid, size_t wait_count)
{
	int	i;
	int	size;
	int	wstatus;

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
			g_exit_status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus))
			ppx_assign_exit_status_when_exited_by_signal(wstatus);
		++i;
	}
	ms_handle_signals_in_parent();
	return ;
}

void	ppx_wait_for_proc_with_heredoc(pid_t pid, size_t *wait_count)
{
	int	wstatus;
	int	status_code;

	wstatus = 0;
	waitpid(pid, &wstatus, WUNTRACED);
	if (WIFEXITED(wstatus))
	{
		status_code = WEXITSTATUS(wstatus);
		g_exit_status = status_code;
	}
	else
		*wait_count += 1;
}
