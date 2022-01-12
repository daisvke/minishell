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
			key_len = ppx_strlen(key);
			return (node->entry + key_len);
		}
		node = node->next;
	}
	return (NULL);
}

int	ms_get_new_expanded_cmd_line_length(t_ms *env, char *cmd_line)
{
	size_t	i;
	size_t	j;
	size_t	len;
	size_t	start;
	char	*key;
	char	*value;
	bool	found_var;

	found_var = false;
	len = 0;
	i = 0;
	while (cmd_line[i])
	{
		if ((i == 0 && cmd_line[i] == '$') \
			|| (i != 0 && cmd_line[i - 1] != '\'' && cmd_line[i] == '$'))
		{
			found_var = true;
			start = i + 1;
			j = start;
			while (cmd_line[j] != ' ' && cmd_line[j] != '\0')
			{
				++i;
				++j;
			}
			key = ms_strdup(&cmd_line[start], j - start); // check error
			value = ms_get_envp_value_from_key(env, key);
			key = ms_free(key);
			if (value)
			{
				len += ppx_strlen(value) - 1;
				i += 1 + j - start;
			}
		}
		else
		{
			++len;
			++i;
		}
	}
	if (found_var == false)
		return (-1);
	return (len);
}

char	*ms_expand_variables(t_ms *env, char *cmd_line)
{
	size_t	i;
	size_t	j;
	size_t	k;
	int		len;
	size_t	start;
	char	*key;
	char	*value;
	char	*new_cmd_line;

	len = ms_get_new_expanded_cmd_line_length(env, cmd_line);
	if (len < 0)
		return (cmd_line);
//	printf("len: %ld\n", len);
	new_cmd_line = malloc(sizeof(char) * (len + 1));
	i = 0;
	k = 0;
	value = NULL;
	while (cmd_line[i])
	{
		if ((i == 0 && cmd_line[i] == '$') \
			|| (i != 0 && cmd_line[i - 1] != '\'' && cmd_line[i] == '$'))
		{
			start = i + 1;
			j = start;
			while (cmd_line[j] != ' ' && cmd_line[j] != '\0')
			{
				++i;
				++j;
			}
			key = ms_strdup(&cmd_line[start], j - start); // check error
			value = ms_get_envp_value_from_key(env, key);
			key = ms_free(key);
			if (value)
				value++;
			//	ppx_memcpy(&new_cmd_line[k], value, ppx_strlen(value));
			while (value && *value)
			{
				new_cmd_line[k] = *value;
				++k;
				value++;
			}
			i += j - start;
		}
		else
		{
			new_cmd_line[k] = cmd_line[i];
			++i;
			++k;
		}
	}
	new_cmd_line[k] = '\0';
	return (new_cmd_line); 
}
