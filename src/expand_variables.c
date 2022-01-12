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
	bool	double_quote;

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
			while (cmd_line[j] != ' ' && cmd_line[j] != '\0' && cmd_line[j] != '\"')
			{
				++i;
				++j;
			}
			double_quote = cmd_line[j] == '\"';
			key = ms_strdup(&cmd_line[start], j - start); // check error
			value = ms_get_envp_value_from_key(env, key);
			key = ms_free(key);
			if (value)
				len += ppx_strlen(value) - 1 + double_quote;
		}
		else
			++len;
		++i;
	}
	if (found_var == false)
		return (-1);
	return (len);
}

int	ms_nbrlen(long long int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
	{
		n = -n;
		len = ms_nbrlen(n) + 1;
	}
	else
	{
		while (n > 0)
		{
			len += 1;
			n = n / 10;
		}
	}
	return (len);
}

char	*ms_itoa(t_ms *env, int n)
{
	char	*res;
	int		len;
	long	nb;

	nb = n;
	len = ms_nbrlen(nb);
	res = (char *)malloc(sizeof(*res) * (len + 1));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (nb < 0)
		nb = -nb;
	while (len--)
	{
		res[len] = nb % 10 + '0';
		nb /= 10;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
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
	bool	double_quote;

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
			if (cmd_line[i + 1] == ' ' || cmd_line[i + 1] == '\0')
				return ("$");
			start = i + 1;
			if (cmd_line[i + 1] == '?' && (cmd_line[i + 2] == ' ' || cmd_line[i + 2] == '\0'))
			{
				value = ms_itoa(env, env->last_pipe_exit_status);
				printf("%s\n", value);
				return (NULL);
			}
			else
			{
				j = start;
				while (cmd_line[j] != ' ' && cmd_line[j] != '\0' && cmd_line[j] != '\"')
				{
					++i;
					++j;
				}
				double_quote = cmd_line[j] == '\"';
				key = ms_strdup(&cmd_line[start], j - start); // check error
				value = ms_get_envp_value_from_key(env, key);
				key = ms_free(key);
				if (value)
					value++;
			}
			while (value && *value)
			{
				new_cmd_line[k] = *value;
				++k;
				value++;
			}
		}
		else
		{
			new_cmd_line[k] = cmd_line[i];
			++k;
		}
		++i;
	}
	new_cmd_line[k] = '\0';
//	printf("str: %s\n", new_cmd_line);
	return (new_cmd_line); 
}
