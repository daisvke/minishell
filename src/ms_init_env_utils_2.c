/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 03:59:23 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/11 04:49:05 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_res	ms_handle_empty_node(
	t_ms *env, char *key, t_env_lst *node, char **entry)
{
	t_res	result;

	result.malloced = false;
	result.key_len = 0;
	if (ms_strcmp(key, "SESSION_MANAGER=local/") == MS_SAME)
	{
		node = ms_lst_get_node_with_the_same_key(
				env->envp_lst, "SESSION_MANAGER="
				);
	}
	if (node == NULL)
		*entry = ms_handle_case_null(&result.key_len, &result.malloced);
	else
	{
		*entry = ms_strdup(node->entry, ms_strlen(node->entry));
		result.malloced = true;
		result.key_len = 16;
	}
	return (result);
}
