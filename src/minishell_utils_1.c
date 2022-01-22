/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:42:49 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/22 05:36:13 by dtanigaw         ###   ########.fr       */
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
	write(STDOUT_FILENO, MS_PROMPT, 16);
	*read_line = readline(NULL);
	if (*read_line == NULL)
		return (MS_READ_EOF);
	if (*read_line[0] == '\0')
	{
		if (read_line)
			read_line = ms_free(*read_line);
		return (MS_READ_NONE);
	}
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
	new = ms_get_new_cmd_line_with_expanded_variables(env, cmd_line);
	if (new == NULL)
		return (1);
	env->split_cmd_line = ms_split_and_activate_options(env, new, '|');
	new = ms_free(new);
	if (env->split_cmd_line == NULL)
		ms_exit_with_error_message(env, 0);
	return (MS_SUCCESS);
}

void	ms_prompt_and_execute_cmd_line_with_pipex(t_ms *env)
{
	size_t	res;
	char	*read_line;
	char	**tmp;

	res = ms_show_prompt_and_read_cmd_line(&read_line);
	if (res == MS_READ_EOF)
	{
		rl_clear_history();
		ms_lst_clear_list(env->envp_lst);
		exit(EXIT_SUCCESS);
	}
	env->cmd_line = read_line;
	if (res == MS_READ_NONE)
		return ;
	if (ms_parse_cmd_line(env, &env->cmd_line) == 1)
	{
		tmp = ms_malloc(env, 2, sizeof(char *));
		tmp[0] = ms_strdup(" ", 2); 
		*tmp[1] = '\0';
		env->ppx_env.cmd = tmp; //or set on a bool and not to free
		return ;
	}
	ms_execute_cmdline_with_pipex(env, env->split_cmd_line);
}
