/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 10:19:54 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/07 11:42:21 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_set_var_according_to_envp_entry(t_ms *env, char **var, char *key)
{
	t_env_lst	*node;
	char		**entry;
	size_t		key_len;
	
	entry = NULL;
	node = ms_lst_get_node_with_the_same_key(env->envp_lst, key);
	if (node == NULL)
	{
		*entry = "";
		key_len = 0;
	}
	else
	{
		entry = &node->entry;
		key_len = ms_strlen(key);
	}
	if (*var)
		ms_free(*var);
	*var = ms_strdup(\
		*entry + key_len, \
		ms_strlen(*entry) - key_len \
	); //check err
}

void	ms_set_first_part_of_cmd_prompt(t_ms *env, t_prompt *cmd_prompt)
{
	char	*first_part;
	char	*last_part;
	char	*colored_first_part;
	char	*colored_last_part;

	ms_set_var_according_to_envp_entry(env, &cmd_prompt->logname, "LOGNAME=");
	ms_set_var_according_to_envp_entry(env, &cmd_prompt->name, "NAME=");
	first_part = ppx_join_three_str(\
		&env->ppx_env, \
		cmd_prompt->logname, "@", cmd_prompt->name
	);//ms vers ?
	colored_first_part = ms_color_string(env, first_part, MS_CLR_BRIGHT_GREEN);
	first_part = ms_free(first_part);
	last_part = cmd_prompt->prompt;
	colored_last_part = ms_color_string(env, last_part, MS_CLR_BOLD_BLUE);
	cmd_prompt->prompt= ppx_join_three_str(\
		&env->ppx_env, \
		colored_first_part, ":", colored_last_part
	);//ms vers ?
	last_part = ms_free(last_part);
	colored_first_part = ms_free(colored_first_part);
	colored_last_part = ms_free(colored_last_part);
}

void	ms_init_cmd_prompt(t_ms *env)
{
	char		*home_path;
	
	//if home = null, look for all cases like this
	home_path = ms_get_home_value_from_envp_lst(env);
	env->cmd_prompt.home_path_len = ms_strlen(home_path);
	home_path = ms_free(home_path);
	ms_generate_new_path_for_prompt(\
		env, NULL, MS_PMP_AT_HOME, MS_PMP_FIRST_TIME \
	);
	ms_set_first_part_of_cmd_prompt(env, &env->cmd_prompt);
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
	pwd = ms_strdup(path_node->entry, ms_strlen(path_node->entry)); //check err
	key_len = 4;
	joined = ppx_join_three_str(&env->ppx_env, "HOME=", pwd + key_len, "");
	pwd = ms_free(pwd);
	ms_free(home_node->entry);
	home_node->entry = ms_strdup(joined, ms_strlen(joined));
	joined = ms_free(joined);
	ms_init_cmd_prompt(env);
}
