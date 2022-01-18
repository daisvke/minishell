/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 01:34:45 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 05:59:38 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ppx_check_access(char *path)
{
	if (access(path, F_OK) == MS_SUCCESS \
		&& access(path, X_OK) == MS_SUCCESS)
		return (MS_SUCCESS);
	return (MS_ERROR);
}

t_env_lst	*ms_lst_get_node_with_the_same_key(t_env_lst *envp_lst, char *key)
{
	t_env_lst	*node;

	node = envp_lst;
	while (node)
	{
		if (ms_compare_with_envp_key(node->entry, key, true) == MS_SAME)
			return (node);
		node = node->next;
	}
	return (NULL);
}

static char	**ppx_get_path(t_ms *ms_env, t_ppx *ppx_env, char *key)
{
	char	*paths_envp;
	char	**paths_envp_split;

	paths_envp = ms_lst_get_node_with_the_same_key(\
			ms_env->envp_lst, key \
		)->entry;
	if (paths_envp == NULL)
		ppx_exit_with_error_message(ppx_env, 9);
	paths_envp_split = ppx_split(paths_envp, ':');
	if (!paths_envp_split)
		ppx_exit_with_error_message(ppx_env, 7);
	return (paths_envp_split);
}

char	*ppx_get_the_right_cmd_path(t_ms *ms_env, t_ppx *ppx_env, \
	char *key, char *cmd)
{
	char	**paths_envp_split;
	char	*cmd_path_at_i;
	size_t	i;

	if (ppx_check_access(cmd) == MS_SUCCESS)
		return (cmd);
	paths_envp_split = ppx_get_path(ms_env, ppx_env, key);
	i = 0;
	cmd_path_at_i = NULL;
	while (paths_envp_split[i])
	{
		cmd_path_at_i = ppx_join_three_str(\
			ppx_env, paths_envp_split[i], "/", cmd);
		if (ppx_check_access(cmd_path_at_i) == MS_SUCCESS)
			break ;
		cmd_path_at_i = ms_free(cmd_path_at_i);
		cmd_path_at_i = NULL;
		++i;
	}
	if (cmd_path_at_i == NULL)
		cmd_path_at_i = cmd;
	ppx_free_split(paths_envp_split);
	paths_envp_split = NULL;
	return (cmd_path_at_i);
}
