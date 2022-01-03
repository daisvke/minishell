/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 01:34:45 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/03 10:35:01 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

bool	ppx_check_access(char *path)
{
	return (access(path, F_OK) == SUCCESS && access(path, X_OK) == SUCCESS);
}

t_envp_data	ppx_get_key_value_from_envp(char *envp[], char *key)
{
	t_envp_data	data;
	size_t		i;
	size_t		key_len;

	data.index = 0;
	data.value = NULL;
	key_len = ppx_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ppx_strncmp(envp[i], key, key_len) == SAME)
		{
			data.index = i;
			data.value = envp[i] + key_len;
			break ;
		}
		++i;
	}
	return (data);
}

static char	**ppx_get_path(t_env_lst *envp_lst, t_ppx *env, char *key)
{
	char	*paths_envp;
	char	**paths_envp_split;

	paths_envp = ppx_get_key_value_from_envp(envp_lst, key).value;
	if (!paths_envp)
		ppx_exit_with_error_message(env, 9);
	paths_envp_split = ppx_split(paths_envp, ':');
	if (!paths_envp_split)
		ppx_exit_with_error_message(env, 7);
	return (paths_envp_split);
}

char	*ppx_get_the_right_cmd_path(t_ppx *env, t_env_lst *envp_lst, \
	char *key, char *cmd)
{
	char	**paths_envp_split;
	char	*cmd_path_at_i;
	size_t	i;

	if (ppx_check_access(cmd) == OK)
		return (cmd);
	paths_envp_split = ppx_get_path(envp_lst, env, key);
	i = 0;
	cmd_path_at_i = NULL;
	while (paths_envp_split[i])
	{
		cmd_path_at_i = ppx_join_three_str(env, paths_envp_split[i], "/", cmd);
		if (ppx_check_access(cmd_path_at_i) == OK)
			break ;
		free(cmd_path_at_i);
		cmd_path_at_i = NULL;
		++i;
	}
	if (cmd_path_at_i == NULL)
		cmd_path_at_i = cmd;
	ppx_free_split(paths_envp_split);
	paths_envp_split = NULL;
	return (cmd_path_at_i);
}
