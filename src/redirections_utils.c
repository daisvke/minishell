/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 11:18:52 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/22 20:15:53 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_apply_append_mode(t_ppx *env, char *file)
{
	int	fd;
	int	open_flags;

	env->options |= MS_OPT_APPEND_OUTPUT;
	ppx_close(env, env->pipe_fds[env->i][1]);
	open_flags = ppx_get_open_flags(env);
	fd = ppx_open_file(env, file, open_flags, 0664);
	ppx_dup2(env, fd, STDOUT_FILENO);
}

char	*ppx_check_outfile(\
	t_ppx *env, char *file, size_t i, size_t *lines_to_del)
{
	*lines_to_del = 1;
	if (*file == '\0' || *file == '>')
	{
		if (env->cmd[i + 1] == NULL)
			ms_print_error_message(19);
		file = env->cmd[i + 1];
		*lines_to_del = 2;
		return (file);
	}
	else
		return (file);
}

void	ppx_putstr_fd(char *s, int fd, bool option)
{
	if (s)
	{
		write(fd, s, ms_strlen(s));
		if (option == MS_PUT_NEWLINE)
			write(fd, "\n", 1);
	}
}

int	ppx_open_file(t_ppx *env, char *file_name, int flags, int mod)
{
	int		fd;
	char	*err_message;

	if (mod)
		fd = open(file_name, flags, mod);
	else
		fd = open(file_name, flags);
	if (fd == PPX_ERROR)
	{
		err_message = strerror(errno);
		ppx_putstr_fd("pipex: ", STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(err_message, STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(": ", STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(file_name, STDERR_FILENO, MS_PUT_NEWLINE);
		ppx_free_pipe_fds(env);
		exit(EXIT_FAILURE);
	}
	return (fd);
}
