/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 11:20:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/19 10:49:23 by dtanigaw         ###   ########.fr       */
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
	while (env->cmd[i])
	{
		if (del.line == i \
			&& ms_check_if_char_is_a_redir_symbol(env->cmd[i][0]) == false \
			&& env->cmd[i][1] != '\0')
			new_cmd_array[j] = ms_strdup(env->cmd[i], del.pos);
		else if ((del.lines_to_del == 1 && i != del.line) \
			|| (del.lines_to_del == 2 && i != del.line && i != del.line + 1))
		{
			new_cmd_array[j] = ms_strdup(env->cmd[i], ms_strlen(env->cmd[i]));
			++j;
		}
		++i;
	}
	if (new_cmd_array && new_cmd_array[0] && *new_cmd_array[0] == '\0')
		new_cmd_array = ms_free(new_cmd_array);
	return (new_cmd_array);
}

char	**ppx_del_redirection_section_at_i(t_ppx *env, t_del del)
{
	size_t	len;
	char	**new_cmd_array;

	len = 1;
	while (env->cmd[len])
		++len;
	if (len == del.lines_to_del)
		len += 1;
	len -= del.lines_to_del;
	new_cmd_array = malloc(sizeof(char *) * (len + 1));
	ms_memset(new_cmd_array, 0, sizeof(char **) * (len + 1));
	new_cmd_array = ppx_del_redirection_section_iter(env, del, new_cmd_array);
	if (*new_cmd_array)
		printf("dsdqsdqsd here\n");
	size_t i;
	for (i=0;new_cmd_array[i];++i)
		printf("new: |%s|\n", new_cmd_array[i]);
	return (new_cmd_array);
}

void	ppx_apply_redirection(t_ppx *env, char *str, char *file)
{
	int	fd;
	int	open_flags;

	fd = 0;
	if (*str == '<' && *(str + 1) == '<')
		ms_apply_heredoc(env, file);
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
	t_del	del;

	file = ms_search_redir_symbol(&env->cmd[i][j]);
	file = ppx_check_outfile(env, file, i, &del.lines_to_del);
	ppx_apply_redirection(env, &env->cmd[i][j], file);
	del.line = i;
	del.pos = j;
	env->cmd = ppx_del_redirection_section_at_i(env, del);
	if (*env->cmd)
		printf("==============\n");
}

void	ppx_handle_redirections(t_ppx *env)
{
	size_t	i;
	size_t	j;

	for(i=0;env->cmd[i];++i)
		printf("cmd;: |%s|\n", env->cmd[i]);
	i = 0;
	while (env->cmd[i])
	{
		j = 0;
		while (*env->cmd && env->cmd[i][j])//added first cond
		{
printf("cccc: %c\n", env->cmd[i][j]);
			if (ms_search_redir_symbol(&env->cmd[i][j]))
			{
				ppx_check_and_apply_redirection(env, i, j);
				i = 0;
				j = 0;
			}
			else
				++j;
	//		printf("cmd: |%c| i:%ld, j:%ld\n",env->cmd[i][j],i,j);
		}
		++i;
	}
}
