/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_proc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/16 22:53:57 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_handle_pipe_in_child_proc(t_ppx *env)
{
	if ((env->options & MS_OPT_HEREDOC) == false \
		&& (env->options & MS_OPT_READ_FROM_FILE) == false \
		&& env->i > 0)
		ppx_dup2(\
			env, env->pipe_fds[env->i - 1][0], STDIN_FILENO, MS_DUP_CLOSE_FD \
		);
	if ((env->options & MS_OPT_REDIR_OUTPUT) == false \
		&& env->i < env->cmd_nbr - 1)
		ppx_dup2(env, env->pipe_fds[env->i][1], STDOUT_FILENO, MS_DUP_OFF);
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
	{
		ppx_free_array_of_pointers(&envp, lst_size);
		ppx_exit_when_cmd_not_found(ms_env, ppx_env->cmd[0]);
	}
}

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	cmd_code;

	if (ppx_env->options & MS_OPT_PIPE)
		ppx_handle_pipe_in_child_proc(ppx_env);
	ppx_handle_redirections(ppx_env);
	if (ppx_env->cmd == NULL || *ppx_env->cmd == NULL)
	{
		ppx_close_pipe_fds(ppx_env);
		ppx_free_pipe_fds(ppx_env);
		ms_lst_clear_list(ms_env->envp_lst);
		ms_free_prompt_struct(ms_env);
		ppx_free_array_of_pointers(&ms_env->split_cmdline, MS_ALL);
		return ;
	}
	if (ms_check_if_the_cmd_is_implemented(\
			ppx_env->cmd, &cmd_code, PPX_PROC_CHILD \
		) == true)
		ms_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd);
	else
		ppx_execute_unimplemented_cmd(ms_env, ppx_env);
	ppx_close_pipe_fds(ppx_env);
	rl_clear_history();
	ms_lst_clear_list(ms_env->envp_lst);
	ms_free_prompt_struct(ms_env);
	ppx_free_all_allocated_variables(ppx_env);
}
