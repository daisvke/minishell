/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:11 by dtanigaw          #+#    #+#             */
/*   Updated: 2025/01/17 02:50:27 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_lst_lstsize(t_env_lst *head)
{
	t_env_lst	*node;
	size_t		i;

	node = head;
	i = 0;
	while (node)
	{
		node = node->next;
		++i;
	}
	return (i);
}

t_env_lst	*ms_lst_get_last_node(t_env_lst *node)
{
	if (node)
		while (node->next)
			node = node->next;
	return (node);
}

void	ms_lst_del_node(t_env_lst *node)
{
	if (node->entry)
		node->entry = ms_free(node->entry);
	if (node)
		node = ms_free(node);
}

void	ms_lst_clear_list(t_env_lst *node)
{
	t_env_lst	*nxt;

	if (!node)
		return ;
	while (node)
	{
		nxt = node->next;
		ms_lst_del_node(node);
		node = nxt;
	}
	node = NULL;
}

char	*ms_get_home_value_from_envp_lst(t_ms *env)
{
	char		*home_path;
	t_env_lst	*home_node;

	home_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "HOME=");
	if (home_node == NULL)
		home_path = getcwd(NULL, 0);
	else
	{
		home_path = ms_strdup(home_node->entry + MS_ENV_HOME_KEYLEN, \
			ms_strlen(home_node->entry) - MS_ENV_HOME_KEYLEN);
		if (home_path == NULL)
			exit(EXIT_FAILURE);
	}
	return (home_path);
}
