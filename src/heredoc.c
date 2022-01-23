/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 03:34:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/23 11:14:43 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ppx_request_heredoc_input(t_ppx *env, char *limiter)
{
	int		fd;
	char	*line;

	line = NULL;
	fd = ppx_open_file(env, "heredoc_tmp", \
		O_CREAT | O_WRONLY | O_TRUNC, 0664);
	ppx_dup2(env, fd, STDOUT_FILENO);
	while (get_next_line(STDIN_FILENO, &line) >= 0)
	{
		if (ms_strncmp(line, limiter, ms_strlen(line)) == MS_SAME)
		{
			line = ms_free(line);
			ppx_close(env, fd);
			return ;
		}
		ppx_putstr_fd(line, STDOUT_FILENO, MS_NONE);
		ppx_putstr_fd("\n", STDOUT_FILENO, MS_NONE);
		line = ms_free(line);
	}
	line = NULL;
	ppx_close(env, fd);
//	ppx_exit_with_error_message(env, 6);
}

void	ms_apply_heredoc(t_ppx *env, char *file)
{
	int	fd;

	env->options |= MS_OPT_HEREDOC; // utile ?
	ppx_request_heredoc_input(env, file);
	fd = ppx_open_file(env, "heredoc_tmp", O_RDONLY, 0);
	ppx_dup2(env, env->pipe_fds[env->i][1], STDOUT_FILENO);
	ppx_dup2(env, fd, STDIN_FILENO);
	unlink("heredoc_tmp");
}
