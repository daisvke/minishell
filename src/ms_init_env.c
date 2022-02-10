/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 10:19:54 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/10 02:58:09 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_handle_case_null(size_t *key_len, bool *malloced)
{
	char	*entry;

	entry = ms_strdup("", 1);
	if (entry == NULL)
		exit(EXIT_FAILURE);
	*malloced = true;
	*key_len = 0;
	return (entry);
}

void	ms_set_var_according_to_envp_entry(t_ms *env, char **var, char *key)
{
	size_t		i;
	size_t		colon_pos;
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
	if (*var)
		ms_free(*var);
	if (ms_strcmp(key, "SESSION_MANAGER=local/") == MS_SAME)
	{
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
		*var = ms_strdup(entry + key_len, ms_strlen(entry) - key_len - (ms_strlen(entry) - colon_pos));
		if (*var == NULL)
			exit(EXIT_FAILURE);
	}
	else
	{
		*var = ms_strdup(entry + key_len, ms_strlen(entry) - key_len);
		if (*var == NULL)
			exit(EXIT_FAILURE);
	}
	if (malloced == true)
		entry = ms_free(entry);
}

void	ms_set_variables_for_cmd_prompt(t_ms *env, t_prompt *cmd_prompt)
{
	ms_set_var_according_to_envp_entry(env, &cmd_prompt->logname, "LOGNAME=");
	ms_set_var_according_to_envp_entry(env, &cmd_prompt->name, "NAME=");
	if (*cmd_prompt->name == '\0')
		ms_set_var_according_to_envp_entry(env, &cmd_prompt->name, "SESSION_MANAGER=local/");
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
	first_part = ppx_join_three_str(\
		&env->ppx_env, \
		cmd_prompt->logname, "@", cmd_prompt->name
	);
	colored_first_part = ms_color_string(env, first_part, MS_CLR_BRIGHT_GREEN);
	first_part = ms_free(first_part);
	last_part = cmd_prompt->prompt;
	colored_last_part = ms_color_string(env, last_part, MS_CLR_BOLD_BLUE);
	cmd_prompt->prompt= ppx_join_three_str(\
		&env->ppx_env, \
		colored_first_part, ":", colored_last_part
	);
	last_part = ms_free(last_part);
	colored_first_part = ms_free(colored_first_part);
	colored_last_part = ms_free(colored_last_part);
}

void	ms_init_cmd_prompt(t_ms *env)
{
	char		*home_path;
	
	home_path = ms_get_home_value_from_envp_lst(env);
	env->cmd_prompt.home_path_len = ms_strlen(home_path);
	home_path = ms_free(home_path);
	ms_generate_new_path_for_prompt(\
		env, NULL, MS_PMP_AT_HOME, MS_PMP_FIRST_TIME \
	);
	ms_set_first_part_of_cmd_prompt(env, &env->cmd_prompt, true);
}

void	ms_init_env(char *envp[], t_ms *env)
{
	t_env_lst	*home_node;
	t_env_lst	*path_node;
	char		*pwd;
	char		*joined;
	size_t		key_len;

	ms_convert_envp_into_linked_list(envp, env);
	home_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "HOME=");
	path_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "PWD=");
	if (path_node == NULL)
	{
		pwd = ms_strdup("", 0);
		if (pwd == NULL)
			exit(EXIT_FAILURE);
		key_len = 0;
	}
	else
	{
		pwd = ms_strdup(path_node->entry, ms_strlen(path_node->entry)); //check err
		if (home_node->entry == NULL)
			ms_exit_with_error_message(env, 11);
		key_len = 4;
	}
	joined = ppx_join_three_str(&env->ppx_env, "HOME=", pwd + key_len, "");
	pwd = ms_free(pwd);
	ms_free(home_node->entry);
	home_node->entry = ms_strdup(joined, ms_strlen(joined));
	if (home_node->entry == NULL)
		ms_exit_with_error_message(env, 11);
	joined = ms_free(joined);
	ms_init_cmd_prompt(env);
}
