
#include "minishell.h"

void	ms_lst_assign_entry_to_node(t_env_lst *node, char *entry)
{
	size_t	len;

	if (node->entry)
		free(node->entry);
	len = ppx_strlen(entry);
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

t_env_lst	*ms_lst_get_last_node(t_env_lst *node)
{
	if (node)
		while (node->next)
			node = node->next;
	return (node);
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
