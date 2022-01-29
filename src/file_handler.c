/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 23:18:51 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/28 23:20:22 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ppx_check_outfile(\
	t_ppx *env, char *file, size_t i, size_t *lines_to_del)
{
	*lines_to_del = 1;
	if (*file == '\0' || *file == '>')
	{
		if (env->cmd[i + 1] == NULL)
			ppx_exit_with_error_message(env, 3);
		file = env->cmd[i + 1];
		*lines_to_del = 2;
		return (file);
	}
	else
		return (file);
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
		ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(err_message, STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(": ", STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(file_name, STDERR_FILENO, MS_PUT_NEWLINE);
		ppx_free_pipe_fds(env);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	ppx_get_open_flags(t_ppx *env)
{
	int	flags;

	flags = 0;
	if (env->options & MS_OPT_APPEND_OUTPUT)
		flags = O_CREAT | O_WRONLY | O_APPEND;
	else if (env->options & MS_OPT_REDIR_OUTPUT)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	return (flags);
}

char	*ppx_generate_filename(t_ppx *env, bool increment)
{
	static size_t	i;
	char			*fileno;
	char			*file;

	fileno = ppx_itoa(env, i);
	file = ppx_join_three_str(env, ".", fileno, "heredoc.tmp");
	fileno = ms_free(fileno);
	if (increment == true)
		++i;
	return (file);
}
