/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 06:19:18 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/27 06:19:30 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ms_get_array_of_error_messages(char *errors[])
{
	errors[0] = "";
	errors[1] = "malloc failed";
	errors[2] = "syntax error near unexpected token `|'";
	errors[3] = "getcwd failed";
	errors[4] = "dup2 failed"; 
	errors[5] = "close failed";
	errors[6] = "get_next_line failed";
	errors[7] = "split failed";
	errors[8] = "permission denied:";
	errors[9] = "execve failed: No such file or directory";
	errors[10] = "getcwd failed";
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
	ppx_putstr_fd("minishell: ", STDERR_FILENO, NONE);
	ppx_putstr_fd(err_message, STDERR_FILENO, PUT_NEWLINE);
}

void	ms_exit_with_error_message(t_ms *env, int err_code)
{
	ms_print_error_message(err_code);
	if (err_code > 0)
	{
		rl_clear_history();
		ppx_free_array_of_pointers(env->ppx_env.cmd, 0);
		ppx_free_pipe_fds(&env->ppx_env);
	}
	exit(EXIT_FAILURE);
}
