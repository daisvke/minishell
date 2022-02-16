/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 03:34:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/16 00:48:15 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_request_heredoc_input(t_ppx *env, char *limiter)
{
	int		fd;
	char	*line;
	char	*file;
	int		stdout_cpy;

	line = NULL;
	file = ppx_generate_filename(env, false);
	fd = ppx_open_file(env, file, \
		O_CREAT | O_WRONLY | O_TRUNC, 0664);
	file = ms_free(file);
	stdout_cpy = dup(STDOUT_FILENO);
	ppx_dup2(env, fd, STDOUT_FILENO, MS_DUP_CLOSE_FD);
	while (get_next_line(STDIN_FILENO, &line) >= 0)
	{
		if (ms_strcmp(line, limiter) == MS_SAME)
		{
			line = ms_free(line);
			ppx_dup2(env, stdout_cpy, STDOUT_FILENO, MS_DUP_CLOSE_FD);
			return ;
		}
		ppx_putstr_fd(line, STDOUT_FILENO, MS_PUT_NEWLINE);
		line = ms_free(line);
	}
	line = ms_free(line);
	line = NULL;
	ppx_dup2(env, stdout_cpy, STDOUT_FILENO, MS_DUP_CLOSE_FD);
}

void	ms_apply_heredoc(t_ppx *env, char *file, size_t hd_pos, size_t hd_total)
{
	char	*in_file;
	int		fd;

	ppx_request_heredoc_input(env, file);
	in_file = ppx_generate_filename(env, false);
	fd = ppx_open_file(env, in_file, O_RDONLY, 0);
	in_file = ms_free(in_file);
	if (hd_pos > 0 || hd_total == 1)
		ppx_dup2(env, fd, STDIN_FILENO, MS_DUP_CLOSE_FD);
	in_file = ppx_generate_filename(env, true);
	unlink(in_file);
	in_file = ms_free(in_file);
}

void	ppx_detect_heredocs(t_ppx *env, char *cmd[])
{
	size_t	i;
	size_t	j;

	env->heredoc_pos = 0;
	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (*cmd && cmd[i][j])
		{
			if (cmd[i][j] == '<' && cmd[i][j + 1] == '<')
			{
				env->options |= MS_OPT_HEREDOC;
				env->heredoc_pos = i;
				return ;
			}
			else
				++j;
		}
		++i;
	}
}

size_t	ppx_count_heredoc(char *cmd[])
{
	size_t	i;
	size_t	j;
	size_t	heredoc_nbr;

	heredoc_nbr = 0;
	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (*cmd && cmd[i][j])
		{
			if (ms_search_redir_symbol(&cmd[i][j]))
			{
				if (cmd[i][j] == '<' && cmd[i][j + 1] == '<')
					++heredoc_nbr;
			}
			++j;
		}
		++i;
	}
	return (heredoc_nbr);
}
