/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 03:34:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/28 05:35:23 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ppx_get_open_flags(t_ppx *env)
{
	int	flags;

	flags = 0;
	if ((env->options & MS_OPT_HEREDOC) \
		&& env->pos == env->argc - GET_LAST_CMD)
		flags = O_CREAT | O_WRONLY | O_APPEND;
	else if (env->pos == env->argc - GET_LAST_CMD)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	return (flags);
}

void	ppx_input_heredoc(t_ppx *env, char *argv[])
{
	int		fd;
	char	*line;
	char	*limiter;

	line = NULL;
	limiter = argv[2];
	fd = ppx_open_file(env, "heredoc_output", \
		O_CREAT | O_WRONLY | O_TRUNC, 0664);
	ppx_dup2(env, fd, 1);
	while (get_next_line(0, &line) >= 0)
	{
		if (ppx_strncmp(line, limiter, ppx_strlen(limiter)) == SAME)
		{
			free(line);
			line = NULL;
			ppx_close(env, fd);
			return ;
		}
		ppx_putstr_fd(line, 1, NONE);
		ppx_putstr_fd("\n", 1, NONE);
		free(line);
	}
	line = NULL;
	ppx_close(env, fd);
	ppx_exit_with_error_message(env, 6);
}
