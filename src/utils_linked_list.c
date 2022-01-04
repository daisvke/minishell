
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
		len = ppx_strlen(node->entry);
		array[i] = ms_strdup(node->entry, len);
		node = node->next;
		++i;
	}
	array[i] = NULL;
	return (array);
}

