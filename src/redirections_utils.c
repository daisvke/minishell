/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 11:18:52 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 00:03:16 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_apply_append_mode(t_ms *ms_env, t_ppx *ppx_env, char *file)
{
	int	fd;
	int	open_flags;

	ppx_env->options |= MS_OPT_APPEND_OUTPUT;
//	ms_close(ms_env, ppx_env->pipe_fds[ppx_env->i][1]);
	open_flags = ppx_get_open_flags(ppx_env);
	fd = ppx_open_file(ppx_env, file, open_flags, 0664);
	ppx_dup2(ppx_env, fd, STDOUT_FILENO, MS_DUP_CLOSE_FD);
}

void	ms_read_from_file(t_ppx *env, char *file)
{
	int	fd;

	env->options |= MS_OPT_READ_FROM_FILE;
	fd = ppx_open_file(env, file, O_RDONLY, 0);
	ppx_dup2(env, fd, STDIN_FILENO, MS_DUP_CLOSE_FD);
}

void	ms_redirect_output(t_ppx *env, char *file)
{
	int	fd;
	int	open_flags;

	env->options |= MS_OPT_REDIR_OUTPUT;
	open_flags = ppx_get_open_flags(env);
	fd = ppx_open_file(env, file, open_flags, 0664);
	ppx_dup2(env, fd, STDOUT_FILENO, MS_DUP_CLOSE_FD);
}

bool	ppx_is_a_line_to_del_not_starting_with_a_redir_symbol(\
	t_del del, size_t i, t_ppx *env)
{
	return (del.line == i \
		&& ms_check_if_char_is_a_redir_symbol(env->cmd[i][0]) == false \
		&& env->cmd[i][1] != '\0');
}

bool	ppx_is_not_a_line_to_del(t_del del, size_t i)
{
	return ((del.lines_to_del == 1 && i != del.line) \
		|| (del.lines_to_del == 2 && i != del.line && i != del.line + 1));
}
