/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 10:19:54 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/11 04:49:05 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_assign_session_manager_value_to_cmd_prompt(\
	char **var, char *entry, size_t key_len)
{
	size_t		i;
	size_t		colon_pos;

	i = 0;
	colon_pos = 0;
	while (entry[i])
	{
		if (entry[i] == ':')
		{
			colon_pos = i;
			break ;
		}
		++i;
	}
	*var = ms_strdup(\
		entry + key_len, \
		ms_strlen(entry) - key_len - (ms_strlen(entry) - colon_pos) \
		);
	if (*var == NULL)
		exit(EXIT_FAILURE);
}

void	ms_set_var_according_to_envp_entry(t_ms *env, char **var, char *key)
{
	t_env_lst	*node;
	char		*entry;
	size_t		key_len;
	bool		malloced;

	entry = NULL;
	malloced = false;
	node = ms_lst_get_node_with_the_same_key(env->envp_lst, key);
	if (node == NULL)
		entry = ms_handle_case_null(&key_len, &malloced);
	else
	{
		entry = node->entry;
		key_len = ms_strlen(key);
	}
	ms_free(*var);
	if (ms_strcmp(key, "SESSION_MANAGER=local/") == MS_SAME)
		ms_assign_session_manager_value_to_cmd_prompt(var, entry, key_len);
	else
		ms_assign_key_value_to_cmd_prompt(var, entry, key_len);
	if (malloced == true)
		entry = ms_free(entry);
}

void	ms_set_first_part_of_cmd_prompt(\
	t_ms *env, t_prompt *cmd_prompt, bool first_time)
{
	char	*first_part;
	char	*last_part;
	char	*colored_first_part;
	char	*colored_last_part;

	if (first_time == true)
		ms_set_variables_for_cmd_prompt(env, cmd_prompt);
	first_part = ppx_join_three_str(&env->ppx_env, \
		cmd_prompt->logname, "@", cmd_prompt->name);
	colored_first_part = ms_color_string(env, first_part, MS_CLR_BRIGHT_GREEN);
	first_part = ms_free(first_part);
	last_part = cmd_prompt->prompt;
	colored_last_part = ms_color_string(env, last_part, MS_CLR_BOLD_BLUE);
	cmd_prompt->prompt = ppx_join_three_str(&env->ppx_env, \
		colored_first_part, ":", colored_last_part);
	last_part = ms_free(last_part);
	colored_first_part = ms_free(colored_first_part);
	colored_last_part = ms_free(colored_last_part);
}

void	ms_set_home_variable_in_envp_lst(t_ms *env)
{
	t_env_lst	*home_node;
	t_env_lst	*path_node;
	char		*pwd;
	char		*joined;
	size_t		key_len;

	key_len = 0;
	home_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "HOME=");
	path_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "PWD=");
	pwd = ms_get_pwd(env, path_node, &key_len);
	joined = ppx_join_three_str(&env->ppx_env, "HOME=", pwd + key_len, "");
	pwd = ms_free(pwd);
	ms_free(home_node->entry);
	home_node->entry = ms_strdup_with_exit(env, joined, ms_strlen(joined));
	joined = ms_free(joined);
}

void	ms_init_env(char *envp[], t_ms *env)
{
	ms_convert_envp_into_linked_list(envp, env);
	ms_set_home_variable_in_envp_lst(env);
	ms_init_cmd_prompt(env);
}
