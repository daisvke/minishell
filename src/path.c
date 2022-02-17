/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 01:34:45 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 07:12:01 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ppx_check_access(char *path, bool check_all)
{
	if (access(path, F_OK) == MS_SUCCESS)
	{
		if (check_all == false)
			return (MS_SUCCESS);
	}
	else
		return (MS_ERROR);
	if (access(path, X_OK) == MS_SUCCESS)
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

static char	**ppx_get_path(t_ms *env, char *key)
{
	t_env_lst	*paths_node;
	char		*paths_envp;
	char		**paths_envp_split;

	paths_node = ms_lst_get_node_with_the_same_key(\
			env->envp_lst, key \
		);
	if (paths_node == NULL)
		return (NULL);
	paths_envp = paths_node->entry;
	if (paths_envp == NULL)
		ms_exit_with_error_message(env, 19);
	paths_envp_split = ppx_split(paths_envp, ':');
	if (!paths_envp_split)
		ms_exit_with_error_message(env, 0);
	return (paths_envp_split);
}

char	*ppx_get_the_right_cmd_path(t_ms *ms_env, char *key, char *cmd)
{
	char	**paths_envp_split;
	char	*cmd_path_at_i;
	size_t	i;

	if (ppx_check_access(cmd, PPX_CHECK_ALL) == MS_SUCCESS)
		return (cmd);
	paths_envp_split = ppx_get_path(ms_env, key);
	i = 0;
	cmd_path_at_i = NULL;
	while (paths_envp_split && paths_envp_split[i])
	{
		cmd_path_at_i = ppx_join_three_str(\
			ms_env, paths_envp_split[i], "/", cmd);
		if (ppx_check_access(cmd_path_at_i, PPX_CHECK_ALL) == MS_SUCCESS)
			break ;
		cmd_path_at_i = ms_free(cmd_path_at_i);
		cmd_path_at_i = NULL;
		++i;
	}
	if (cmd_path_at_i == NULL)
		cmd_path_at_i = cmd;
	ppx_free_array_of_pointers(&paths_envp_split, MS_ALL);
	paths_envp_split = NULL;
	return (cmd_path_at_i);
}
