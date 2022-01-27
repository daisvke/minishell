/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 11:20:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/27 08:53:40 by dtanigaw         ###   ########.fr       */
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
		{
			new_cmd_array[j++] = ppx_strdup_with_exit(\
				env, env->cmd[i], del.pos);
		}
		else if (ppx_is_not_a_line_to_del(del, i))
		{
			new_cmd_array[j++] = ppx_strdup_with_exit(\
				env, env->cmd[i], ms_strlen(env->cmd[i]));
		}
		++i;
	}
	if ((new_cmd_array && new_cmd_array[0] && *new_cmd_array[0] == '\0') \
		|| (*new_cmd_array == NULL))
		new_cmd_array = ms_free(new_cmd_array);
	if (new_cmd_array)
		new_cmd_array[j] = NULL;
/*
int fd;
	for (fd=0;new_cmd_array[fd];++fd)
		printf("cmd: %s\n",new_cmd_array[fd]);
*/
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

void	ppx_apply_redirection(t_ppx *env, char *str, char *file)
{
	static size_t	hd_count;
	size_t			hd_total;
	int				fd;
	int				open_flags;

	fd = 0;
	if (*str == '<' && *(str + 1) == '<')
	{
		hd_total = ppx_count_heredoc(env->cmd);
		ms_apply_heredoc(env, file, hd_count, hd_total);
		++hd_count;
	}
	else if (*str == '>' && *(str + 1) == '>')
		ms_apply_append_mode(env, file);
	else if (*str == '<')
	{
		env->options |= MS_OPT_READ_FROM_FILE;
		fd = ppx_open_file(env, file, O_RDONLY, 0);
		ppx_dup2(env, fd, STDIN_FILENO);
	}
	else if (*str == '>')
	{
		env->options |= MS_OPT_REDIR_OUTPUT;
		open_flags = ppx_get_open_flags(env);
		fd = ppx_open_file(env, file, open_flags, 0664);
		ppx_dup2(env, fd, STDOUT_FILENO);
	}
}

void	ppx_check_and_apply_redirection(t_ppx *env, size_t i, size_t j)
{
	char	*file;
	char	**cmd_line_without_redir_section;
	t_del	del;

	file = ms_search_redir_symbol(&env->cmd[i][j]);
	file = ppx_check_outfile(env, file, i, &del.lines_to_del);
	ppx_apply_redirection(env, &env->cmd[i][j], file);
	del.line = i;
	del.pos = j;
	cmd_line_without_redir_section = ppx_del_redirection_section_at_i(env, del);
	ppx_free_array_of_pointers(&env->cmd, MS_ALL);
	env->cmd = cmd_line_without_redir_section;
}

void	ppx_handle_redirections(t_ppx *env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env->cmd[i])
	{
		j = 0;
		while (*env->cmd && env->cmd[i][j])
		{
			if (ms_search_redir_symbol(&env->cmd[i][j]))
			{
				ppx_check_and_apply_redirection(env, i, j);
				if (env->cmd == NULL)
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
