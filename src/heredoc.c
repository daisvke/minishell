/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 03:34:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/02 01:27:10 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_request_heredoc_input(t_ppx *env, char *limiter)
{
	int		fd;
	char	*line;
	char	*file;

printf("lim: |%s|",limiter);
	line = NULL;
	file = ppx_generate_filename(env, false);
	fd = ppx_open_file(env, file, \
		O_CREAT | O_WRONLY | O_TRUNC, 0664);
	file = ms_free(file);
	ppx_dup2(env, fd, STDOUT_FILENO, MS_DUP_CLOSE_FD);
	while (get_next_line(env, STDIN_FILENO, &line) >= 0)
	{
		if (ms_strcmp(line, limiter) == MS_SAME)
		{
			line = ms_free(line);
//	ppx_close(env, STDIN_FILENO);
//	ppx_close(env, STDIN_FILENO);
			perror("cmp");
			return ;
		}
		ppx_putstr_fd(line, STDOUT_FILENO, MS_PUT_NEWLINE);
		line = ms_free(line);
	}
//	ppx_close(env, STDIN_FILENO);
	line = NULL;
}

void	ms_apply_heredoc(t_ppx *env, char *file, size_t hd_pos, size_t hd_total)
{
	char	*in_file;
	int		fd;
	int		stdout_cpy;
	int		stdin_cpy;

	stdout_cpy = dup(STDOUT_FILENO);
//	stdin_cpy = dup(STDIN_FILENO);
	ppx_request_heredoc_input(env, file);
//	ppx_dup2(env, stdin_cpy, STDIN_FILENO, MS_DUP_CLOSE_FD);//close ?
	in_file = ppx_generate_filename(env, false);
	fd = ppx_open_file(env, in_file, O_RDONLY, 0);
	in_file = ms_free(in_file);
//	if (hd_total != 1 = !(hd_total > 1 && hd_pos == hd_total - 1))
//		ppx_dup2(env, env->pipe_fds[env->i][1], STDOUT_FILENO, MS_DUP_CLOSE_FD);
	if (hd_pos > 0 || hd_total == 1)
		ppx_dup2(env, fd, STDIN_FILENO, MS_DUP_CLOSE_FD);
	in_file = ppx_generate_filename(env, true);
	unlink(in_file);
	in_file = ms_free(in_file);
	ppx_close(env, STDOUT_FILENO);
	ppx_dup2(env, stdout_cpy, STDOUT_FILENO, MS_DUP_CLOSE_FD);//close ?
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
