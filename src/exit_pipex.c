/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 23:18:50 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 01:57:38 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_free_all_allocated_variables(t_ppx *env)
{
	ppx_free_array_of_pointers(&env->cmd, MS_ALL);
	ppx_free_pipe_fds(env);
}

void	ppx_exit_when_cmd_not_found(t_ms *env, char *cmd)
{
	ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
	ppx_putstr_fd(cmd, STDERR_FILENO, MS_NONE);
	ppx_putstr_fd(": command not found", STDERR_FILENO, MS_PUT_NEWLINE);
	ms_close_pipe_fds(env, &env->ppx_env, MS_CPF_OFF);
	ppx_free_pipe_fds(&env->ppx_env);
	ms_lst_clear_list(env->envp_lst);
	ms_free_prompt_struct(env);
	ppx_free_array_of_pointers(&env->split_cmdline, MS_ALL);
	ppx_free_array_of_pointers(&env->ppx_env.cmd, MS_ALL);
	exit(127);
}
