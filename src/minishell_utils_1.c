/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:42:49 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/19 12:38:23 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

void	ms_use_argv_to_create_cmdline(int argc, char *argv[], t_ms *env)
{
	int		i;
	size_t	j;

	env->split_cmdline = ms_malloc(env, argc, sizeof(char *));
	i = MS_FIRST_ARG_POS;
	j = 0;
	while (i < argc)
	{
		env->split_cmdline[j] = ms_strdup(argv[i], ms_strlen(argv[i]));
		if (env->split_cmdline[j] == NULL)
		{
			ppx_free_array_of_pointers(&env->split_cmdline, j);
			ms_exit_with_error_message(env, 11);
		}
		++j;
		++i;
	}
	env->split_cmdline[j] = NULL;
}

int	ms_prompt_user_and_read_cmdline(t_ms *env, char **read_line)
{
	size_t	i;
	char	*read_line_cpy;

	*read_line = readline(env->cmd_prompt.prompt);
	if (*read_line == NULL)
		return (MS_READ_EOF);
	if (*read_line[0] == '\0')
		return (MS_READ_NONE);
	read_line_cpy = (char *)*readline;
	i = 0;
	while (read_line_cpy[i])
	{
		if (i > 2147483647)
			ms_exit_with_error_message(env, 17);
		++i;
	}
	add_history(*read_line);
	return (MS_READ_LINE);
}

char	*ms_get_new_cmdline_with_expanded_variables(\
	t_ms *env, char **cmdline, t_expv *vars)
{
	char	*new;

	new = ms_expand_variables(env, *cmdline, vars);
	if (vars->found_var == true)
		*cmdline = ms_free(*cmdline);
	return (new);
}

int	ms_parse_cmdline(t_ms *env, char **cmdline)
{
	int		err_code;
	char	*new;
	t_expv	vars;

	err_code = ms_check_if_quote_nbr_is_even(*cmdline);
	if (err_code != MS_SUCCESS)
		return (err_code);
	err_code = ms_check_pipes_and_redirections(env, *cmdline);
	if (err_code != MS_SUCCESS)
		return (err_code);
	ms_memset(&vars, 0, sizeof(t_expv));
	new = ms_get_new_cmdline_with_expanded_variables(env, cmdline, &vars);
	if (new == NULL)
		return (3);
	env->split_cmdline = ms_split_and_activate_options(env, new, '|');
	if (vars.found_var == true)
		new = ms_free(new);
	if (env->split_cmdline == NULL)
		ms_exit_with_error_message(env, 7);
	return (MS_SUCCESS);
}

int	ms_run_readline(t_ms *env, char *read_line)
{
	size_t	res;

	res = ms_prompt_user_and_read_cmdline(env, &read_line);
	if (res == MS_READ_EOF)
		ms_quit_with_ctrl_d(env);
	if (res == MS_READ_NONE)
	{
		read_line = ms_free(read_line);
		return (MS_ERROR);
	}
	env->cmdline = read_line;
	return (MS_SUCCESS);
}
