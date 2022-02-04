/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 04:39:25 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/04 01:19:45 by dtanigaw         ###   ########.fr       */
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
	if (ppx_env->options & MS_OPT_PIPE \
		&& ppx_env->i != ppx_env->cmd_nbr - 1)
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

void	ppx_wait_for_all_children(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t pid, size_t wait_count)
{
	int	i;
	int	size;
	int	wstatus = 0;
	int	status_code;

	i = 0;
	size = ppx_env->cmd_nbr - wait_count;
	while (i < size)
	{
		if (waitpid(pid, &wstatus, WUNTRACED) == PPX_ERROR)
		{
			ppx_free_all_allocated_variables(&ms_env->ppx_env);
			ppx_free_array_of_pointers(&ms_env->split_cmd_line, MS_ALL);
		}
		if (WIFEXITED(wstatus))
		{
			ms_env->exit_status = MS_EXIT_SUCCESS;
			status_code = WEXITSTATUS(wstatus);
			ms_env->last_pipe_exit_status = wstatus; //or status_code ?
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

	int	status_code;
	int	wstatus = 0;
	size_t	wait_count = 0;

//pos= i ?
	while (ppx_env->i < ppx_env->cmd_nbr)
	{
		ppx_env->options &= MS_OPT_INIT_ALL_BUT_PIPE;
		if (ppx_create_array_of_commands(ms_env, ppx_env, cmd_line) == 2)
			return ;
		ppx_detect_heredocs(ppx_env, ppx_env->cmd);
		if (ppx_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == true)
			ppx_execute_implemented_cmd_in_parent(\
				ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else if (ms_check_if_the_cmd_is_implemented(\
			ppx_env->cmd, &cmd_code, PPX_PROC_PARENT) == true)
			continue ;
		else
			ppx_execute_pipe_and_run_cmd_in_child_process(\
				ms_env, ppx_env, &pid);
		if (ppx_env->i < ppx_env->cmd_nbr - 1)
			ppx_free_array_of_pointers(&ppx_env->cmd, MS_ALL);

		if (ppx_env->options & MS_OPT_HEREDOC \
			&& ppx_env->i == ppx_env->heredoc_pos)
		{
			ms_env->exit_status = MS_EXIT_SUCCESS;
			waitpid(pid, &wstatus, WUNTRACED);
			if (WIFEXITED(wstatus))
			{
				status_code = WEXITSTATUS(wstatus);
				ms_env->last_pipe_exit_status = status_code;
			}
			else
				++wait_count;
		}
		++ppx_env->i;
		++ppx_env->i;
	}
	if (ppx_env->cmd_nbr > 1)
		ms_close_pipe_fds(ms_env, ppx_env, MS_CPF_AFTER_INCREM);
	if (ppx_env->cmd == NULL && ms_free(ms_env->cmd_line) == NULL)
		return ;
	if (ppx_pipe_is_off_and_cmd_is_implemented(ppx_env, &cmd_code) == false)
		ppx_wait_for_all_children(ms_env, ppx_env, pid, wait_count);
}

void	ms_execute_cmdline_with_pipex(t_ms *env, char **cmd_line)
{
	size_t	i;
	size_t	cmd_and_file_nbr;

	i = 0;
	while (cmd_line[i])
		++i;
	cmd_and_file_nbr = i;
	ppx_init_ppx(env, &env->ppx_env, cmd_and_file_nbr);
	ppx_pipex(env, &env->ppx_env, cmd_line);
}
