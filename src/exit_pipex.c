/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 23:18:50 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/13 10:11:29 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ppx_get_array_of_error_messages(char *errors[])
{
	errors[0] = "split: failed to allocate memory";
	errors[1] = "malloc: failed to allocate memory";
	errors[2] = "permission denied:";
	errors[3] = "execve failed: No such file or directory";
	errors[4] = "dup2 failed";
	errors[5] = "close failed";
	errors[6] = "get_next_line failed";
	errors[7] = "strdup: failed to allocate memory";
	errors[8] = "itoa: failed to allocate memory";
	return (errors);
}

char	*ppx_get_err_message_from_err_code(int err_code)
{
	char	*err_messages[8 + 1];

	ppx_get_array_of_error_messages(err_messages);
	return (err_messages[err_code]);
}

void	ppx_free_all_allocated_variables(t_ppx *env)
{
	ppx_free_array_of_pointers(&env->cmd, MS_ALL);
	ppx_free_pipe_fds(env);
}

void	ppx_exit_with_error_message(t_ppx *env, int err_code)
{
	char	*err_message;

	err_message = NULL;
	err_message = ppx_get_err_message_from_err_code(err_code);
	ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
	ppx_putstr_fd(err_message, STDERR_FILENO, MS_PUT_NEWLINE);
	ppx_free_all_allocated_variables(env);
	exit(EXIT_FAILURE);
}

void	ppx_exit_when_cmd_not_found(t_ms *env, char *cmd, char *path_to_cmd)
{
	ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
	ppx_putstr_fd(cmd, STDERR_FILENO, MS_NONE);
	ppx_putstr_fd(": command not found", STDERR_FILENO, MS_PUT_NEWLINE);
	ppx_close_pipe_fds(&env->ppx_env);
	path_to_cmd = ms_free(path_to_cmd);
	ppx_free_pipe_fds(&env->ppx_env);
	ms_lst_clear_list(env->envp_lst);
	ms_free_prompt_struct(env);
	ppx_free_array_of_pointers(&env->split_cmdline, MS_ALL);
	exit(EXIT_FAILURE);
}
