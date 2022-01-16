/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_proc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 09:56:30 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_handle_pipe_in_child_proc(t_ms *ms_env, t_ppx *ppx_env)
{
	ppx_close(ppx_env, ppx_env->pipe_fds[ppx_env->i][0]);
	ppx_dup2(ppx_env, ppx_env->fd_in, STDIN_FILENO);
	if (ppx_env->pos != ppx_env->cmd_nbr - 1 \
		&& (ms_env->options & MS_OPT_REDIR_OUTPUT) == false)
		ppx_dup2(ppx_env, ppx_env->pipe_fds[ppx_env->i][1], STDOUT_FILENO);
}

void	ms_execute_implemented_cmd(\
	t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[])
{
	if (cmd_code == MS_CMD_ECHO)
		ms_execute_cmd_echo(ppx_env->cmd);
	else if (cmd_code == MS_CMD_PWD)
		ms_execute_cmd_pwd(ms_env->envp_lst);
	else if (cmd_code == MS_CMD_ENV)
		ms_execute_cmd_env(ms_env->envp_lst);
	ppx_execute_implemented_cmd_in_parent(ms_env, ppx_env, cmd_code, cmd);
}

void	ppx_execute_unimplemented_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	lst_size;
	char	**envp;
	char	*path_to_cmd;

	path_to_cmd = ppx_get_the_right_cmd_path(\
			ms_env, ppx_env, "PATH=", ppx_env->cmd[0] \
		);
	lst_size = ms_lst_lstsize(ms_env->envp_lst);
	envp = ms_convert_envp_lst_to_array_of_pointers(\
		ms_env, ms_env->envp_lst, lst_size);
	if (execve(path_to_cmd, ppx_env->cmd, envp) == PPX_ERROR)
		ppx_exit_when_cmd_not_found(ppx_env, ppx_env->cmd[0]);
	ppx_free_array_of_pointers(envp, lst_size);
}

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	cmd_code;

	ppx_handle_redirections(ppx_env);
	if (ppx_env->cmd)
	{
		if (ppx_env->options & MS_OPT_PIPE)
			ppx_handle_pipe_in_child_proc(ms_env, ppx_env);
		if (ms_check_if_the_cmd_is_implemented(\
				ppx_env->cmd, &cmd_code, PPX_PROC_CHILD \
			) == true)
			ms_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else
			ppx_execute_unimplemented_cmd(ms_env, ppx_env);
	}
}
