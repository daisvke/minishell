/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 21:38:53 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/24 02:57:41 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_get_envp_value_from_key(t_ms *env, char *key)
{
	t_env_lst	*node;
	size_t		key_len;

	node = env->envp_lst;
	while (node)
	{
		if (ms_compare_with_envp_key(node->entry, key, false) == MS_SAME)
		{
			key_len = ms_strlen(key);
			return (node->entry + key_len);
		}
		node = node->next;
	}
	return (NULL);
}

void	ms_copy_value_to_the_expansion_location(\
	char *value, char *location, t_expv *vars)
{
	while (value && *value)
	{
		location[vars->k] = *value;
		++vars->k;
		value++;
	}
}

char	*ms_print_last_exit_status(t_ms *env)
{
	char	*value;

	value = ms_itoa(env, env->last_pipe_exit_status);
	value = ms_free(value);
	return (NULL);
}

bool	ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
	char *cmd_line, t_expv *vars)
{
	return ((vars->i == 0 && cmd_line[vars->i] == '$') \
			|| (vars->i != 0 \
			&& cmd_line[vars->i - 1] != '\'' && cmd_line[vars->i] == '$'));
}
