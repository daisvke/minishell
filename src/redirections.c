/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 11:20:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 11:32:40 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ppx_del_redirection_section_at_i(\
	t_ppx *env, size_t del_line, size_t del_pos, size_t lines_to_del)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	**new_argv;

	i = 0;
	len = 1;
	while (env->cmd[len])
		++len;
	if (len == lines_to_del)
		len += 1;
	len -= lines_to_del;
	new_argv = malloc(sizeof(char *) * (len + 1));
	ms_memset(new_argv, 0, sizeof(char **) * (len + 1));
	i = 0;
	j = 0;
	while (env->cmd[i])
	{
		if (del_line == i \
			&& ms_check_if_char_is_a_redir_symbol(env->cmd[i][0]) == false \
			&& env->cmd[i][1] != '\0')
			new_argv[j] = ms_strdup(env->cmd[i], del_pos);
		else if ((lines_to_del == 1 && i != del_line) \
			|| (lines_to_del == 2 && i != del_line && i != del_line + 1))
		{
			new_argv[j] = ms_strdup(env->cmd[i], ms_strlen(env->cmd[i]));
			++j;
		}
		++i;
	}
	if (*new_argv[0] == '\0')
		new_argv = ms_free(new_argv);
	return (new_argv);
}

void	ppx_apply_redirection(t_ppx *env, char *str, char *file)
{
	int	fd;
	int	open_flags;

	fd = 0;
	if (*str == '<' && *(str + 1) == '<')
	{
		env->options |= MS_OPT_HEREDOC;
		ppx_request_heredoc_input(env, file);
		fd = ppx_open_file(env, "heredoc_tmp", O_RDONLY, 0);
		ppx_dup2(env, env->pipe_fds[env->i][1], STDOUT_FILENO);
		ppx_dup2(env, fd, STDIN_FILENO);
		unlink("heredoc_tmp");
	}
	else if (*str == '>' && *(str + 1) == '>')
	{
		env->options |= MS_OPT_APPEND_OUTPUT; 
		ppx_close(env, env->pipe_fds[env->i][1]);
		open_flags = ppx_get_open_flags(env);
		fd = ppx_open_file(env, file, open_flags, 0664);
		ppx_dup2(env, fd, STDOUT_FILENO);
	}
	else if (*str == '<')
	{
		env->options |= MS_OPT_READ_FROM_FILE;
		fd = ppx_open_file(env, file, O_RDONLY, 0);
		ppx_dup2(env, fd, STDIN_FILENO);
	}
	else if (*str == '>')
	{
		env->options |= MS_OPT_REDIR_OUTPUT;
		ppx_close(env, env->pipe_fds[env->i][1]);
		open_flags = ppx_get_open_flags(env);
		fd = ppx_open_file(env, file, open_flags, 0664);
		ppx_dup2(env, fd, STDOUT_FILENO);
	}
}

void	ppx_handle_redirections(t_ppx *env)
{
	size_t	i;
	size_t	j;
	char	*file;
	size_t	lines_to_del;

	i = 0;
	while (env->cmd[i])
	{
		j = 0;
		while (env->cmd[i][j])
		{
			if (ms_search_redir_symbol(&env->cmd[i][j]))
			{
				file = ms_search_redir_symbol(&env->cmd[i][j]);
				file = ppx_check_outfile(env, file, i, &lines_to_del);
				ppx_apply_redirection(env, &env->cmd[i][j], file);
				env->cmd = ppx_del_redirection_section_at_i(\
					env, i, j, lines_to_del);
				i = 0;
				j = 0;
			}
			else
				++j;
		}
		++i;
	}
}
