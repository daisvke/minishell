
#include "minishell.h"

void	ms_lst_assign_entry_to_node(t_env_lst *node, char *entry)
{
	size_t	i;
	size_t	equal_len;
	size_t	key_len;
	size_t	val_len;

	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	i = 0;
	while (entry[i] != '=')
		++i;
	key_len = i;
	node->key = ms_strdup(entry, key_len);
	equal_len = 1;
	val_len = ppx_strlen(entry) - key_len - equal_len;
	node->value = ms_strdup(entry + key_len + equal_len, val_len);
}

t_env_lst	*ms_lst_create_new_node(char *data)
{
	t_env_lst	*new;

	new = malloc(sizeof(t_env_lst));
	if (!new)
		return (NULL);
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
