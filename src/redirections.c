/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 11:20:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 00:46:03 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ppx_del_redirection_section_iter(\
	t_ppx *env, t_del del, char *new_cmd_array[])
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (env->cmd && env->cmd[i])
	{
		if (ppx_is_a_line_to_del_not_starting_with_a_redir_symbol(del, i, env) \
			== true)
			new_cmd_array[j++] \
				= ppx_strdup_with_exit(env, env->cmd[i], del.pos);
		else if (ppx_is_not_a_line_to_del(del, i))
			new_cmd_array[j++] = ppx_strdup_with_exit(\
				env, env->cmd[i], ms_strlen(env->cmd[i]));
		++i;
	}
	if ((new_cmd_array && new_cmd_array[0] && *new_cmd_array[0] == '\0') \
		|| (*new_cmd_array == NULL))
		new_cmd_array = ms_free(new_cmd_array);
	if (new_cmd_array)
		new_cmd_array[j] = NULL;
	return (new_cmd_array);
}

char	**ppx_del_redirection_section_at_i(t_ppx *env, t_del del)
{
	size_t	len;
	bool	first_char_is_not_a_redir_symbol;
	char	**new_cmd_array;

	len = 1;
	while (env->cmd[len])
		++len;
	if (len == del.lines_to_del)
		len += 1;
	first_char_is_not_a_redir_symbol = \
		ms_check_if_char_is_a_redir_symbol(env->cmd[del.line][0]) == false ;
	len += first_char_is_not_a_redir_symbol - del.lines_to_del;
	new_cmd_array = malloc(sizeof(char *) * (len + 1));
	ms_memset(new_cmd_array, 0, sizeof(char **) * (len + 1));
	new_cmd_array = ppx_del_redirection_section_iter(\
		env, del, new_cmd_array);
	return (new_cmd_array);
}

void	ppx_apply_redirection(t_ms *ms_env, t_ppx *ppx_env, char *str, char *file)
{
	static size_t	hd_pos;
	size_t			hd_total;
	int				fd;

	fd = 0;
	if (*str == '<' && *(str + 1) == '<')
	{
		hd_total = ppx_count_heredoc(ppx_env->cmd);
		ms_apply_heredoc(ms_env, file, hd_pos, hd_total);
		++hd_pos;
	}
	else if (*str == '>' && *(str + 1) == '>')
		ms_apply_append_mode(ms_env, ppx_env, file);
	else if (*str == '<')
		ms_read_from_file(ms_env, ppx_env, file);
	else if (*str == '>')
		ms_redirect_output(ms_env, ppx_env, file);
}

void	ppx_check_and_apply_redirection(t_ms *ms_env, t_ppx *ppx_env, size_t i, size_t j)
{
	char	*file;
	char	**cmdline_without_redir_section;
	t_del	del;

	file = ms_search_redir_symbol(&ppx_env->cmd[i][j]);
	file = ppx_check_outfile(ppx_env, file, i, &del.lines_to_del);
	ppx_apply_redirection(ms_env, ppx_env, &ppx_env->cmd[i][j], file);
	del.line = i;
	del.pos = j;
	cmdline_without_redir_section = ppx_del_redirection_section_at_i(ppx_env, del);
	ppx_free_array_of_pointers(&ppx_env->cmd, MS_ALL);
	ppx_env->cmd = cmdline_without_redir_section;
}

void	ppx_handle_redirections(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (ppx_env->cmd[i])
	{
		j = 0;
		while (*ppx_env->cmd && ppx_env->cmd[i][j])
		{
			if (ms_search_redir_symbol(&ppx_env->cmd[i][j]))
			{
				ppx_check_and_apply_redirection(ms_env, ppx_env, i, j);
				if (ppx_env->cmd == NULL)
					return ;
				i = -1;
				break ;
			}
			else
				++j;
		}
		++i;
	}
}
