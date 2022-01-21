/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:15:05 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/21 12:01:19 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ms_get_expanded_value_from_cmd_line(\
	t_ms *env, char *cmd_line, t_expv *vars, int mode)
{
	char	*key;
	char	*value;

	vars->j = vars->start;
	while (cmd_line[vars->j] != ' ' && cmd_line[vars->j] != '\0' \
		&& cmd_line[vars->j] != '\"')
	{
		++vars->i;
		++vars->j;
	}
	vars->double_quote = cmd_line[vars->j] == '\"';
	key = ms_strdup(&cmd_line[vars->start], vars->j - vars->start);
	if (key == NULL)
		ms_exit_with_error_message(env, 11);
	value = ms_get_envp_value_from_key(env, key);
	key = ms_free(key);
	if (value)
	{
		vars->found_var = true;
		if (mode == 1)
			value++;
		else
			vars->len += ms_strlen(value) - 1 + vars->double_quote;
	}
	return (value);
}

bool	ms_found_last_pipe_exit_status_symbols(char *cmd_line, t_expv *vars)
{
	if (cmd_line[vars->i] == '$' && cmd_line[vars->i + 1] == '?')
	{
		vars->found_var = true;
		return (true);
	}
	return (false);
}

void	ms_get_last_pipe_exit_status_length(t_ms *env, t_expv *vars)
{
	size_t	len;

	len = ms_nbrlen(env->last_pipe_exit_status);
	vars->len += len - 1;
}

int	ms_get_new_expanded_cmd_line_length(t_ms *env, char *cmd_line)
{
	t_expv	vars;

	ms_memset(&vars, 0, sizeof(t_expv));
	while (cmd_line[vars.i])
	{
		if (ms_found_last_pipe_exit_status_symbols(cmd_line, &vars))
			ms_get_last_pipe_exit_status_length(env, &vars);
		else if (ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
			cmd_line, &vars) == true)
		{
			vars.start = vars.i + 1;
			ms_get_expanded_value_from_cmd_line(env, cmd_line, &vars, 0);
		}
		else
			++vars.len;
		++vars.i;
	}
	if (vars.found_var == false)
		return (-1);
	return (vars.len);
}

char	*ms_expand_last_exit_status_or_value_from_envp(\
	t_ms *env, char *cmd_line, t_expv *vars, char *new_cmd_line)
{
	char	*value;
	static int i;

	vars->status = 0;
	if (cmd_line[vars->i + 1] == ' ' || cmd_line[vars->i + 1] == '\0')
	{
		vars->status = 1;
		ms_free(new_cmd_line);
		cmd_line = ms_free(cmd_line);
		new_cmd_line = ms_strdup("$", 2);
		return (new_cmd_line);
	}
	else
	{
	vars->start = vars->i + 1;
		value = ms_get_expanded_value_from_cmd_line(\
			env, cmd_line, vars, 1);
	}
	ms_copy_value_to_the_expansion_location(value, new_cmd_line, vars);
	++i;
	return (new_cmd_line);
}

void	ms_expand_last_pipe_exit_status(\
	t_ms *env, char *new_cmd_line, t_expv *vars)
{
	char	*exit_status;

	exit_status = ms_itoa(env->last_pipe_exit_status);
	ms_copy_value_to_the_expansion_location(exit_status, new_cmd_line, vars);
	exit_status = ms_free(exit_status);
	++vars->i;
}

char	*ms_expand_variables(t_ms *env, char *cmd_line, t_expv *vars)
{
	int		len;
	char	*new_cmd_line;

	len = ms_get_new_expanded_cmd_line_length(env, cmd_line);
	if (len < 0)
		return (cmd_line);
	new_cmd_line = ms_malloc(env, len + 1, sizeof(char));
	while (cmd_line[vars->i])
	{
		if (ms_found_last_pipe_exit_status_symbols(cmd_line, vars))
			ms_expand_last_pipe_exit_status(env, new_cmd_line, vars);	
		else if (ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
			cmd_line, vars) == true)
		{
			new_cmd_line = ms_expand_last_exit_status_or_value_from_envp(\
				env, cmd_line, vars, new_cmd_line);
			if (vars->status == 1)
				return (new_cmd_line);
		}
		else
			new_cmd_line[vars->k++] = cmd_line[vars->i];
		++vars->i;
	}
	new_cmd_line[vars->k] = '\0';
	if (vars->found_var == false)
		cmd_line = ms_free(cmd_line);
	return (new_cmd_line);
}
