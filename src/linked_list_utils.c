/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:11 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/14 03:16:16 by dtanigaw         ###   ########.fr       */
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
