/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:05 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/14 03:16:09 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_lst_assign_entry_to_node(t_env_lst *node, char *entry)
{
	size_t	len;

	ms_free(node->entry);
	len = ms_strlen(entry);
	node->entry = ms_strdup(entry, len);
}

t_env_lst	*ms_lst_create_new_node(char *data)
{
	t_env_lst	*new;

	new = malloc(sizeof(t_env_lst));
	if (!new)
		return (NULL);
	new->entry = NULL;
	ms_lst_assign_entry_to_node(new, data);
	new->next = NULL;
	return (new);
}

void	ms_lst_add_back(t_env_lst *head, t_env_lst *new)
{
	t_env_lst	*node;

	if (!head || !new)
		return ;
	if (!head)
		head = new;
	else
	{
		node = ms_lst_get_last_node(head);
		node->next = new;
	}
}

char	**ms_convert_envp_lst_to_array_of_pointers(t_env_lst *envp_lst, size_t lst_size)
{
	t_env_lst	*node;
	char		**array;
	size_t		i;
	size_t		len;

	array = malloc(sizeof(char *) * (lst_size + 1));
	node = envp_lst;
	i = 0;
	while (node)
	{
		len = ms_strlen(node->entry);
		array[i] = ms_strdup(node->entry, len);
		node = node->next;
		++i;
	}
	array[i] = NULL;
	return (array);
}

int	ms_compare_with_envp_key(const char *envp_entry, const char *str, bool equal_in_str)
{
	if (str && envp_entry)
	{
		while (*str && *envp_entry)
		{
			if (equal_in_str == true && *envp_entry == '=')
				break ;
			if (*str != *envp_entry)
				return ((unsigned char)(*str) - (unsigned char)(*envp_entry));
			str++;
			envp_entry++;
		}
		if (equal_in_str == true && *str == '=')
			return (MS_SAME);
		else if (equal_in_str == false && *envp_entry == '=')
			return (MS_SAME);
	}
	return (MS_DIFFERENT);
}
