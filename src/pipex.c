/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/18 10:43:06 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_execute_pipe_and_run_cmd_in_child_process(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t *pid)
{
	if (ppx_env->options & MS_OPT_PIPE)
		ms_pipe(ms_env, ppx_env->pipe_fds[ppx_env->i]);
	*pid = ms_fork(ms_env);
	if (*pid == PPX_PROC_CHILD)
	{
		ms_handle_signals_in_child();
		ppx_spawn_child_to_execute_cmd(ms_env, ppx_env);
		exit(EXIT_SUCCESS);
	}
	if (ppx_env->options & MS_OPT_PIPE \
		&& ppx_env->i != ppx_env->cmd_nbr - 1)
		ms_close(ms_env, ppx_env->pipe_fds[ppx_env->i][1]);
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

void	ms_execute_command(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t *pid, size_t *cmd_code)
{
	if (ms_pipe_is_off_and_cmd_is_implemented(ppx_env, cmd_code) == true)
		ppx_execute_implemented_cmd_in_parent(\
			ms_env, ppx_env, *cmd_code, ppx_env->cmd);
	else
		ppx_execute_pipe_and_run_cmd_in_child_process(\
			ms_env, ppx_env, pid);
	if (ppx_env->i < ppx_env->cmd_nbr - 1)
		ppx_free_array_of_pointers(&ppx_env->cmd, MS_ALL);
}

void	ppx_pipe(t_ms *ms_env, t_ppx *ppx_env, char *cmdline[])
{
	pid_t	pid;
	size_t	cmd_code;
	size_t	wait_count;

	wait_count = 0;
	while (ppx_env->i < ppx_env->cmd_nbr)
	{
		ppx_env->options &= MS_OPT_INIT_ALL_BUT_PIPE;
		if (ppx_create_array_of_commands(ms_env, ppx_env, cmdline) == 2)
			return ;
		ppx_detect_heredocs(ppx_env, ppx_env->cmd);
		ms_execute_command(ms_env, ppx_env, &pid, &cmd_code);
		if (ppx_env->options & MS_OPT_HEREDOC \
			&& ppx_env->i == ppx_env->heredoc_pos)
			ppx_wait_for_proc_with_heredoc(pid, &wait_count);
		++ppx_env->i;
	}
	if (ppx_env->cmd_nbr > 1)
		ms_close_pipe_fds(ms_env, ppx_env, MS_CPF_AFTER_INCREM);
	if (ppx_env->cmd == NULL && ms_free(ms_env->cmdline) == NULL)
		return ;
	if (ms_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == false)
		ppx_wait_for_all_children(ms_env, ppx_env, pid, wait_count);
}

void	ms_execute_cmdline_with_pipex(t_ms *env, char **cmdline)
{
	size_t	i;
	size_t	cmd_and_file_nbr;

	i = 0;
	while (cmdline[i])
		++i;
	cmd_and_file_nbr = i;
	ppx_init_ppx(env, &env->ppx_env, cmd_and_file_nbr);
	ppx_pipe(env, &env->ppx_env, cmdline);
}
