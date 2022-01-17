/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 06:19:18 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/17 04:05:09 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ms_get_array_of_error_messages(char *errors[])
{
	errors[0] = "split: failed to allocate memory";
	errors[1] = "malloc failed";
	errors[2] = "syntax error near unexpected token `|'";
	errors[3] = "syntax error near unexpected token `<<'";
	errors[4] = "syntax error near unexpected token `>>'";
	errors[5] = "getcwd failed";
	errors[6] = "minishell: envp is set to NULL";
	errors[7] = "failed to fork child process";
	errors[8] = "close failed";
	errors[9] = "dup2 failed";
	errors[10] = "pipe failed";
	errors[11] = "strdup: failed to allocate memory";
	errors[12] = "ms_lst_create_new_node: failed to allocate memory";
	errors[13] = "waitpid failed";
	errors[14] = "sigaction failed";
	errors[15] = "unclosed quotes are forbidden";
	return (errors);
}

char	*ms_get_err_message_from_err_code(int err_code)
{
	char	*err_messages[MS_ERR_MAX + 1];

	ms_get_array_of_error_messages(err_messages);
	return (err_messages[err_code]);
}

void	ms_print_error_message(int err_code)
{
	char	*err_message;

	err_message = NULL;
	err_message = ms_get_err_message_from_err_code(err_code);
	ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
	ppx_putstr_fd(err_message, STDERR_FILENO, MS_PUT_NEWLINE);
}

void	ms_exit_with_error_message(t_ms *env, int err_code)
{
	ms_print_error_message(err_code);
	rl_clear_history();
	ppx_free_array_of_pointers(env->ppx_env.cmd, 0);
	ppx_free_pipe_fds(&env->ppx_env);
	ms_lst_clear_list(&env->envp_lst);
	exit(EXIT_FAILURE);
}
