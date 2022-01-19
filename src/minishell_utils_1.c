/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:42:49 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/19 23:53:24 by dtanigaw         ###   ########.fr       */
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
	*read_line = readline("\033[0;32m$\033[0;37m ");
	if (*read_line == NULL)
		return (MS_READ_EOF);
	if (*read_line[0] == '\0')
		return (MS_READ_NONE);
	add_history(*read_line);
	return (MS_READ_LINE);
}

int	ms_parse_cmd_line(t_ms *env, char **cmd_line)
{
	int		err_code;
	t_expv	vars;
	char	*new;

	err_code = ms_check_if_quote_nbr_is_even(*cmd_line);
	if (err_code != MS_SUCCESS)
	{
		ms_print_error_message(err_code);
		return (MS_ERROR);
	}
	err_code = ms_check_pipes_and_redirections(env, *cmd_line);
	if (err_code != MS_SUCCESS)
	{
		ms_print_error_message(err_code);
		return (MS_ERROR);
	}
	ms_memset(&vars, 0, sizeof(t_expv));
	new = ms_expand_variables(env, *cmd_line, &vars);
	if (vars.found_var == true)
		*cmd_line = ms_free(*cmd_line);
	if (new == NULL)
		return (1);
	env->split_cmd_line = ms_split_and_activate_options(env, new, '|');
	new = ms_free(new);
	if (env->split_cmd_line == NULL)
		ms_exit_with_error_message(env, 0);
	
	return (MS_SUCCESS);
}

void	ms_prompt_and_execute_cmd_line_with_pipex(\
	char *envp[], t_ms *env, int last_pipe_exit_status)
{
	size_t	res;
	char	*read_line;

	res = ms_show_prompt_and_read_cmd_line(&read_line);
	if (res == MS_READ_EOF)
		exit(EXIT_SUCCESS);
	ms_init_env(envp, env, true);
	env->last_pipe_exit_status = last_pipe_exit_status;
	env->cmd_line = read_line;
	if (res == MS_READ_NONE \
		|| ms_parse_cmd_line(env, &env->cmd_line) == 1)
		return ;
//	printf("prompt cmd: %s\n", env->cmd_line);
	ms_execute_cmdline_with_pipex(env, env->split_cmd_line);
}
