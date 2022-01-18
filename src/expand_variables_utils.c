/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 21:38:53 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 03:50:49 by dtanigaw         ###   ########.fr       */
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
	printf("%s\n", value);
	return (NULL);
}
