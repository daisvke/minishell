/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:42:49 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/23 21:39:53 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmdline_with_pipex(t_ms *env, char **cmd_line)
{
	size_t	i;
	size_t	cmd_and_file_nbr;

	i = 0;
	while (cmd_line[i])
		++i;
	cmd_and_file_nbr = i;
	ppx_init_ppx(env, &env->ppx_env, cmd_and_file_nbr);
	ppx_pipex(env, &env->ppx_env, cmd_line);
}

int	ms_show_prompt_and_read_cmd_line(char **read_line)
{
//	write(STDOUT_FILENO, MS_PROMPT, 16);
	*read_line = readline(MS_PROMPT);
	if (*read_line == NULL)
		return (MS_READ_EOF);
	if (*read_line[0] == '\0')
		return (MS_READ_NONE);
	add_history(*read_line);
	return (MS_READ_LINE);
}

char	*ms_get_new_cmd_line_with_expanded_variables(t_ms *env, char **cmd_line)
{
	char	*new;
	t_expv	vars;

	ms_memset(&vars, 0, sizeof(t_expv));
	new = ms_expand_variables(env, *cmd_line, &vars);
	if (vars.found_var == true)
		*cmd_line = ms_free(*cmd_line);
	return (new);
}

int	ms_parse_cmd_line(t_ms *env, char **cmd_line)
{
	int		err_code;
	char	*new;

	err_code = ms_check_if_quote_nbr_is_even(*cmd_line);
	if (err_code != MS_SUCCESS)
		return (err_code);
	err_code = ms_check_pipes_and_redirections(env, *cmd_line);
	if (err_code != MS_SUCCESS)
		return (err_code);
	new = ms_get_new_cmd_line_with_expanded_variables(env, cmd_line);
	if (new == NULL)
		return (3);
	env->split_cmd_line = ms_split_and_activate_options(env, new, '|');
	new = ms_free(new);
	if (env->split_cmd_line == NULL)
		ms_exit_with_error_message(env, 7);
	return (MS_SUCCESS);
}

int	ms_prompt_and_execute_cmd_line_with_pipex(t_ms *env, char *read_line)
{
	size_t	res;

	res = ms_show_prompt_and_read_cmd_line(&read_line);
	if (res == MS_READ_EOF)
		ms_quit_with_ctrl_d(env);
	if (res == MS_READ_NONE)
	{
		read_line = ms_free(read_line);
		return (MS_ERROR);
	}
	env->cmd_line = read_line;
	return (MS_SUCCESS);
}
