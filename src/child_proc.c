/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_proc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 04:10:33 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_handle_pipe_in_child_proc(t_ms *ms_env, t_ppx *ppx_env)
{
	if ((ppx_env->options & MS_OPT_HEREDOC) == false \
		&& (ppx_env->options & MS_OPT_READ_FROM_FILE) == false \
		&& ppx_env->i > 0)
		ms_dup2(ms_env, \
			ppx_env->pipe_fds[ppx_env->i - 1][0], \
			STDIN_FILENO, MS_DUP_CLOSE_FD \
		);
	if ((ppx_env->options & MS_OPT_REDIR_OUTPUT) == false \
		&& (ppx_env->options & MS_OPT_APPEND_OUTPUT) == false \
		&& ppx_env->i < ppx_env->cmd_nbr - 1)
		ms_dup2(ms_env, \
			ppx_env->pipe_fds[ppx_env->i][1], \
			STDOUT_FILENO, MS_DUP_OFF \
		);
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

void	ms_close_all_standard_fds(t_ms *env)
{
	ms_close(env, STDIN_FILENO);
	ms_close(env, STDOUT_FILENO);
	ms_close(env, STDERR_FILENO);
}

void	ppx_execute_unimplemented_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	lst_size;
	char	**envp;
	char	*path_to_cmd;

	path_to_cmd = ppx_get_the_right_cmd_path(ms_env, "PATH=", ppx_env->cmd[0]);
	lst_size = ms_lst_lstsize(ms_env->envp_lst);
	envp = ms_convert_envp_lst_to_array_of_pointers(\
		ms_env, ms_env->envp_lst, lst_size);
	if (execve(path_to_cmd, ppx_env->cmd, envp) == PPX_ERROR)
	{
		ms_close_all_standard_fds(ms_env);
		ppx_free_array_of_pointers(&envp, lst_size);
		ppx_exit_when_cmd_not_found(ms_env, ppx_env->cmd[0]);
	}
}

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	cmd_code;

	if (ppx_env->options & MS_OPT_PIPE)
		ppx_handle_pipe_in_child_proc(ms_env, ppx_env);
	ppx_handle_redirections(ms_env, ppx_env);
	if (ppx_env->cmd == NULL || *ppx_env->cmd == NULL)
	{
		ms_close_all_standard_fds(ms_env);
		ms_free_all_allocated_variables(ms_env);
		return ;
	}
	if (ms_check_if_the_cmd_is_implemented(\
			ppx_env->cmd, &cmd_code, PPX_PROC_CHILD \
		) == true)
		ms_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd);
	else
		ppx_execute_unimplemented_cmd(ms_env, ppx_env);
	ms_close_all_standard_fds(ms_env);
	ms_free_all_allocated_variables(ms_env);
}
