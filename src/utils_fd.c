/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 13:12:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/29 03:29:33 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ppx_open_file(t_ppx *env, char *file_name, int flags, int mod)
{
	int		fd;
	char	*err_message;

	if (mod)
		fd = open(file_name, flags, mod);
	else
		fd = open(file_name, flags);
	if (fd == ERROR)
	{
		err_message = strerror(errno);
		ppx_putstr_fd("pipex: ", STDERR_FILENO, NONE);
		ppx_putstr_fd(err_message, STDERR_FILENO, NONE);
		ppx_putstr_fd(": ", STDERR_FILENO, NONE);
		ppx_putstr_fd(file_name, STDERR_FILENO, PUT_NEWLINE);
		ppx_free_pipe_fds(env);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	ppx_get_fd(t_ppx *env, char *argv[])
{
	int	fd;
	int	open_flags;

	fd = 0;
	open_flags = ppx_get_open_flags(env);
	if ((env->options & MS_OPT_HEREDOC) \
		&& env->pos == FIRST_CMD_WHEN_HEREDOC)
	{
		fd = ppx_open_file(env, "heredoc_output", O_RDONLY, 0);
		ppx_dup2(env, fd, 0);
	}
	else if ((env->options & MS_OPT_HEREDOC) \
		&& env->pos == FIRST_CMD_WHEN_READING_FROM_FILE)
	{
		fd = ppx_open_file(env, argv[INPUT_FILE], O_RDONLY, 0);
		ppx_dup2(env, fd, 0);
	}
	else if ((env->options & MS_OPT_REDIR_OUTPUT) == false  \
		&& env->pos == env->argc - 1)
	{
		ppx_dup2(env, STDOUT_FILENO, 1);
	}
	else if ((env->options & MS_OPT_REDIR_OUTPUT) \
		&& env->pos == env->argc - GET_LAST_CMD)
	{
		fd = ppx_open_file(env, argv[env->argc - 1], open_flags, 0664);
		ppx_dup2(env, fd, 1);
	}
}

void	ppx_putstr_fd(char *s, int fd, bool option)
{
	if (s)
	{
		write(fd, s, ppx_strlen(s));
		if (option == PUT_NEWLINE)
			write(fd, "\n", 1);
	}
}
