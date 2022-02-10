/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:11 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/10 03:00:50 by dtanigaw         ###   ########.fr       */
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
	node->entry = ms_free(node->entry);
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
	size_t		key_len;

	home_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "HOME=");
	if (home_node == NULL)
		home_path = getcwd(NULL, 0);
	else
	{
		key_len = 5;
		home_path = ms_strdup(home_node->entry + 5, \
			ms_strlen(home_node->entry) - 5);
		if (home_path == NULL)
			exit(EXIT_FAILURE);
	}
	return (home_path);
}
