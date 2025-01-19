/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 03:59:23 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/11 04:49:05 by dtanigaw         ###   ########.fr       */
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

void	ms_assign_key_value_to_cmd_prompt(\
	char **var, char *entry, size_t key_len)
{
	*var = ms_strdup(entry + key_len, ms_strlen(entry) - key_len);
	if (*var == NULL)
		exit(EXIT_FAILURE);
}

void	ms_set_variables_for_cmd_prompt(t_ms *env, t_prompt *cmd_prompt)
{
	ms_set_var_according_to_envp_entry(env, &cmd_prompt->logname, "LOGNAME=");
	ms_set_var_according_to_envp_entry(env, &cmd_prompt->name, "NAME=");
	if (*cmd_prompt->name == '\0')
		ms_set_var_according_to_envp_entry \
		(\
			env, &cmd_prompt->name, "SESSION_MANAGER=local/" \
		);
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

char	*ms_get_pwd(t_ms *env, t_env_lst *path_node, size_t *key_len)
{
	char	*pwd;

	if (path_node == NULL)
	{
		pwd = ms_strdup("", 0);
		if (pwd == NULL)
			exit(EXIT_FAILURE);
	}
	else
	{
		pwd = ms_strdup_with_exit(\
			env, path_node->entry, ms_strlen(path_node->entry));
		*key_len = 4;
	}
	return (pwd);
}
