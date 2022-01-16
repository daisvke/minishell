/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 09:49:31 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_execute_pipe_and_run_cmd_in_child_process(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t *pid)
{
	if (ppx_env->options & MS_OPT_PIPE)
		ppx_pipe(ms_env, ppx_env->pipe_fds[ppx_env->i]);
	*pid = ppx_fork(ms_env);
	if (*pid == PPX_PROC_CHILD)
	{
		ppx_spawn_child_to_execute_cmd(ms_env, ppx_env);
		exit(EXIT_SUCCESS);
	}
	if ((ppx_env->options & MS_OPT_PIPE)
		&& ppx_env->pos != ppx_env->cmd_nbr - 1)
		ppx_save_data_from_child(ms_env, ppx_env);
}

void	ppx_execute_implemented_cmd_in_parent(\
	t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[])
{
	if (cmd_code == MS_CMD_CD)
		ms_execute_cmd_cd(ms_env, ppx_env, cmd[1]);
	else if (cmd_code == MS_CMD_EXPORT)
		ms_execute_cmd_export(ms_env, cmd);
	else if (cmd_code == MS_CMD_UNSET)
		ms_execute_cmd_unset(ms_env, cmd);
	else if (cmd_code == MS_CMD_EXIT)
		exit(EXIT_SUCCESS);
}

void	ppx_wait_for_all_children(t_ms *ms_env, t_ppx *ppx_env, pid_t pid)
{
	int	i;
	int	size;
	int	wstatus;
	int	status_code;

	i = 0;
	size = ppx_env->cmd_nbr;
	while (i < size)
	{
		if (waitpid(pid, &wstatus, WUNTRACED) == PPX_ERROR)
			ms_exit_with_error_message(ms_env, 13);
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

void	ppx_pipex(t_ms *ms_env, t_ppx *ppx_env, char *cmd_line[])
{
	pid_t	pid;
	size_t	cmd_code;

	while (ppx_env->pos < ppx_env->cmd_nbr)
	{
		if (ppx_create_array_of_commands(ms_env, ppx_env, cmd_line) \
			== MS_READ_NONE)
			return ;
		add_history(ms_env->cmd_line);
		if (ppx_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == true)
			ppx_execute_implemented_cmd_in_parent(\
				ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else
			ppx_execute_pipe_and_run_cmd_in_child_process(\
				ms_env, ppx_env, &pid);
		++ppx_env->pos;
		++ppx_env->i;
	}
	if (ppx_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == false)
		ppx_wait_for_all_children(ms_env, ppx_env, pid);
	ppx_free_pipe_fds(ppx_env);
}
