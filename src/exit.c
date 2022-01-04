/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 23:18:50 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 02:03:10 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_free_pipe_fds(t_ppx *env)
{
	int	size;
	int	i;

	if (!env->cmd_nbr)
		return ;
	size = env->cmd_nbr;
	i = 0;
	while (i < size)
	{
		free(env->pipe_fds[i]);
		++i;
	}
	free(env->pipe_fds);
}

char	**ppx_get_array_of_error_messages(char *errors[])
{
	errors[0] = "\nUsage1: ./pipex [input file] [cmd1] [cmd2] [output file]\n" \
		"Usage2: ./pipex here_doc [limiter] [cmd] [cmd1] [output file]";
	errors[1] = "malloc failed";
	errors[2] = "failed to fork child process";
	errors[3] = "pipe failed";
	errors[4] = "dup2 failed";
	errors[5] = "close failed";
	errors[6] = "get_next_line failed";
	errors[7] = "split failed";
	errors[8] = "permission denied:";
	errors[9] = "execve failed: No such file or directory";
	errors[10] = "getcwd failed";
	return (errors);
}

char	*ppx_get_err_message_from_err_code(int err_code)
{
	char	*err_messages[ERR_MAX + 1];

	ppx_get_array_of_error_messages(err_messages);
	return (err_messages[err_code]);
}

void	ppx_exit_with_error_message(t_ppx *env, int err_code)
{
	char	*err_message;

	err_message = NULL;
	err_message = ppx_get_err_message_from_err_code(err_code);
	ppx_putstr_fd("pipex: ", STDERR_FILENO, NONE);
	ppx_putstr_fd(err_message, STDERR_FILENO, PUT_NEWLINE);
	ppx_free_pipe_fds(env);
	exit(EXIT_FAILURE);
}

void	ppx_exit_when_cmd_not_found(t_ppx *env, char *cmd)
{
	ppx_putstr_fd(cmd, STDERR_FILENO, NONE);
	ppx_putstr_fd(": command not found", STDERR_FILENO, PUT_NEWLINE);
	ppx_free_array_of_pointers(env->cmd, 0);
	ppx_free_pipe_fds(env);
	exit(EXIT_FAILURE);
}
