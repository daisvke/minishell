/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:15:05 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/16 23:12:26 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ms_get_expanded_value_from_cmdline(\
	t_ms *env, char *cmdline, t_expv *vars, int mode)
{
	char	*key;
	char	*value;

	vars->j = vars->start;
	while (cmdline[vars->j] != ' ' && cmdline[vars->j] != '\0' \
		&& cmdline[vars->j] != '\"')
	{
		++vars->i;
		++vars->j;
	}
	vars->double_quote = cmdline[vars->j] == '\"';
	key = ms_strdup(&cmdline[vars->start], vars->j - vars->start);
	if (key == NULL)
		ms_exit_with_error_message(env, 11);
	value = ms_get_envp_value_from_key(env, key);
	key = ms_free(key);
	vars->found_var = true;
	if (!value)
		value = " ";
	if (mode == MS_ENV_GET_EXPANDED_VALUE_MODE)
		value++;
	else
		vars->len += ms_strlen(value) - 1 + vars->double_quote;
	return (value);
}

int	ms_get_new_expanded_cmdline_length(t_ms *env, char *cmdline)
{
	t_expv	vars;

	ms_memset(&vars, 0, sizeof(t_expv));
	while (cmdline[vars.i])
	{
		if (ms_found_exit_status_symbols(cmdline, &vars))
			ms_get_exit_status_length(&vars);
		else if (ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
			cmdline, &vars) == true)
		{
			vars.start = vars.i + 1;
			ms_get_expanded_value_from_cmdline(env, cmdline, &vars, 0);
		}
		else
			++vars.len;
		++vars.i;
	}
	if (vars.found_var == false)
		return (-1);
	return (vars.len);
}

char	*ms_expand_exit_status_or_value_from_envp(\
	t_ms *env, char *cmdline, t_expv *vars, char *new_cmdline)
{
	char		*value;
	static int	i;

	vars->status = 0;
	if (cmdline[vars->i + 1] == ' ' || cmdline[vars->i + 1] == '\0')
	{
		vars->status = 1;
		ms_free(new_cmdline);
		cmdline = ms_free(cmdline);
		new_cmdline = ms_strdup("$", 2);
		if (new_cmdline == NULL)
			ms_exit_with_error_message(env, 11);
		return (new_cmdline);
	}
	else
	{
		vars->start = vars->i + 1;
		value = ms_get_expanded_value_from_cmdline(\
			env, cmdline, vars, MS_ENV_GET_EXPANDED_VALUE_MODE);
	}
	ms_copy_value_to_the_expansion_location(value, new_cmdline, vars);
	++i;
	return (new_cmdline);
}

int	ms_fill_new_cmdline(\
	t_ms *env, char *cmdline, t_expv *vars, char *new_cmdline)
{
	while (cmdline[vars->i])
	{
		if (ms_found_exit_status_symbols(cmdline, vars))
			ms_expand_exit_status(env, new_cmdline, vars);
		else if (ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
			cmdline, vars) == true)
		{
			new_cmdline = ms_expand_exit_status_or_value_from_envp(\
				env, cmdline, vars, new_cmdline);
			if (vars->status == 1)
				return (1);
		}
		else
			new_cmdline[vars->k++] = cmdline[vars->i];
		++vars->i;
	}
	return (0);
}

char	*ms_expand_variables(t_ms *env, char *cmdline, t_expv *vars)
{
	int		len;
	char	*new_cmdline;

	len = ms_get_new_expanded_cmdline_length(env, cmdline);
	if (len < 0)
		return (cmdline);
	new_cmdline = ms_malloc(env, len + 1, sizeof(char));
	if (ms_fill_new_cmdline(env, cmdline, vars, new_cmdline) == 1)
		return (new_cmdline);
	new_cmdline[vars->k] = '\0';
	if (vars->found_var == false)
		cmdline = ms_free(cmdline);
	return (new_cmdline);
}
