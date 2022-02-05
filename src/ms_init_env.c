/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 10:19:54 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/05 11:07:31 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_init_cmd_prompt(t_ms *env)
{
	char		*home_path;
	t_env_lst	*home_node;
	size_t		key_len;
	
	//if home = null, look for all cases like this
	home_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "HOME=");
	key_len = 5;
	home_path = home_node->entry + 5;
	env->cmd_prompt.home_path_len = ms_strlen(home_path);
	ms_generate_new_path_for_prompt(\
		env, NULL, MS_PMP_AT_HOME, MS_PMP_FIRST_TIME \
	);
}

void	ms_init_env(char *envp[], t_ms *env)
{
	t_env_lst	*home_node;
	t_env_lst	*path_node;
	char		*pwd;
	char		*joined;
	size_t		key_len;

	ms_convert_envp_into_linked_list(envp, env);

	//init home
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
