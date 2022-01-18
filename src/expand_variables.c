/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:15:05 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 03:57:50 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_get_new_expanded_cmd_line_length(t_ms *env, char *cmd_line)
{
	size_t	i;
	size_t	j;
	size_t	len;
	size_t	start;
	char	*key;
	char	*value;
	bool	found_var;
	bool	double_quote;

	found_var = false;
	len = 0;
	i = 0;
	while (cmd_line[i])
	{
		if ((i == 0 && cmd_line[i] == '$') \
			|| (i != 0 && cmd_line[i - 1] != '\'' && cmd_line[i] == '$'))
		{
			found_var = true;
			start = i + 1;
			j = start;
			while (cmd_line[j] != ' ' && cmd_line[j] != '\0' && cmd_line[j] != '\"')
			{
				++i;
				++j;
			}
			double_quote = cmd_line[j] == '\"';
			key = ms_strdup(&cmd_line[start], j - start); // check error
			value = ms_get_envp_value_from_key(env, key);
			key = ms_free(key);
			if (value)
				len += ms_strlen(value) - 1 + double_quote;
		}
		else
			++len;
		++i;
	}
	if (found_var == false)
		return (-1);
	return (len);
}

void	*ms_get_expanded_value_from_cmd_line(\
	t_ms *env, char *cmd_line, t_expv *vars)
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
	key = ms_strdup(&cmd_line[vars->start], vars->j - vars->start);
	if (key == NULL)
		ms_exit_with_error_message(env, 11);
	value = ms_get_envp_value_from_key(env, key);
	key = ms_free(key);
	if (value)
		value++;
	return (value);
}

char	*ms_expand_last_exit_status_or_value_from_envp(\
	t_ms *env, char *cmd_line, t_expv *vars, char *new_cmd_line)
{
	char	*value;

	vars->status = 0;
	if (cmd_line[vars->i + 1] == ' ' || cmd_line[vars->i + 1] == '\0')
	{
		vars->status = 1;
		return ("$");
	}
	vars->start = vars->i + 1;
	if (cmd_line[vars->i + 1] == '?' && (cmd_line[vars->i + 2] == ' ' \
		|| cmd_line[vars->i + 2] == '\0'))
	{
		vars->status = 1;
		return (ms_print_last_exit_status(env));
	}
	else
		value = ms_get_expanded_value_from_cmd_line(\
			env, cmd_line, vars);
	ms_copy_value_to_the_expansion_location(value, new_cmd_line, vars);
	return (new_cmd_line);
}

bool	ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
	char *cmd_line, t_expv *vars)
{
	return ((vars->i == 0 && cmd_line[vars->i] == '$') \
			|| (vars->i != 0 \
			&& cmd_line[vars->i - 1] != '\'' && cmd_line[vars->i] == '$'));
}

char	*ms_expand_variables(t_ms *env, char *cmd_line)
{
	t_expv	vars;
	int		len;
	char	*new_cmd_line;

	len = ms_get_new_expanded_cmd_line_length(env, cmd_line);
	if (len < 0)
		return (cmd_line);
	new_cmd_line = malloc(sizeof(char) * (len + 1));
	ms_memset(&vars, 0, sizeof(t_expv));
	while (cmd_line[vars.i])
	{
		if (ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
			cmd_line, &vars) == true)
		{
			new_cmd_line = ms_expand_last_exit_status_or_value_from_envp(\
				env, cmd_line, &vars, new_cmd_line);
			if (vars.status == 1)
				return (new_cmd_line);
		}
		else
			new_cmd_line[vars.k++] = cmd_line[vars.i];
		++vars.i;
	}
	new_cmd_line[vars.k] = '\0';
	return (new_cmd_line);
}
