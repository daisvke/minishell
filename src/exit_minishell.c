/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 06:19:18 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/22 06:26:10 by dtanigaw         ###   ########.fr       */
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
	errors[16] = "itoa: failed to allocate memory";
	errors[17] = "envp is not set";
	errors[18] = "argc < 1";
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

void	ms_free_all_allocated_variables(t_ms *env)
{
	rl_clear_history();
	ms_lst_clear_list(env->envp_lst);
	ppx_free_all_allocated_variables(&env->ppx_env);
	ppx_free_array_of_pointers(&env->split_cmd_line, MS_ALL);
	if (env->cmd_line && *env->cmd_line) //for each ?
		env->cmd_line = ms_free(env->cmd_line);
}

void	ms_exit_with_error_message(t_ms *env, int err_code)
{
	rl_clear_history();
	ms_print_error_message(err_code);
	ms_free_all_allocated_variables(env);
	exit(EXIT_FAILURE);
}

size_t	ms_get_symbol_error_code(char symbol)
{
	if (symbol == '|')
		return (2);
	if (symbol == '<')
		return (3);
	else
		return (4);
}
