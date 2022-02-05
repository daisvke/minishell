/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 01:34:45 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/05 09:49:50 by dtanigaw         ###   ########.fr       */
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

static char	**ppx_get_path(t_ms *ms_env, t_ppx *ppx_env, char *key)
{
	char	*paths_envp;
	char	**paths_envp_split;

	paths_envp = ms_lst_get_node_with_the_same_key(\
			ms_env->envp_lst, key \
		)->entry;
	if (paths_envp == NULL)
		ppx_exit_with_error_message(ppx_env, 3);
	paths_envp_split = ppx_split(paths_envp, ':');
	if (!paths_envp_split)
		ppx_exit_with_error_message(ppx_env, 0);
	return (paths_envp_split);
}

char	*ppx_get_the_right_cmd_path(t_ms *ms_env, t_ppx *ppx_env, \
	char *key, char *cmd)
{
	char	**paths_envp_split;
	char	*cmd_path_at_i;
	size_t	i;

	if (ppx_check_access(cmd, PPX_CHECK_ALL) == MS_SUCCESS)
		return (cmd);
	paths_envp_split = ppx_get_path(ms_env, ppx_env, key);
	i = 0;
	cmd_path_at_i = NULL;
	while (paths_envp_split[i])
	{
		cmd_path_at_i = ppx_join_three_str(\
			ppx_env, paths_envp_split[i], "/", cmd);
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

size_t	ms_get_new_path_length(\
	char *home_path, char *current_path, size_t home_path_len)
{
	size_t	i;
	size_t	curr_path_len;

	curr_path_len = ms_strlen(current_path);
	i = 0;
	while (current_path[i] == home_path[i])
		++i;
	if (i == 0)
		return (curr_path_len);
	if (i == ms_strlen(home_path) && i == curr_path_len)
		return (MS_PMP_AT_HOME);
	else
		return (\
			MS_PMP_TILDE_LEN \
			+ (curr_path_len - home_path_len) \
			+ MS_PMP_DOLLAR_SPACE_LEN);
}

void	ms_generate_new_path_for_prompt(\
	t_ms *env, char *current_path, int len, bool first_time)
{
	char	*new_path;

	if (first_time == false)
		ms_free(env->cmd_prompt.prompt);
	if (len == MS_PMP_AT_HOME)
	{
		len = 2;
		env->cmd_prompt.prompt = ms_strdup("~$ ", 3);
	}
	else
	{
		new_path = ppx_join_three_str(\
			&env->ppx_env, \
			"~", \
			current_path + env->cmd_prompt.home_path_len, \
			"$ "
		);//ms vers ?
		env->cmd_prompt.prompt = new_path;
	}
}

void	ms_get_new_path_for_prompt(\
	t_ms *env, t_env_lst *envp_lst, t_prompt *cmd_prompt)
{
	t_env_lst	*pwd_node;
	t_env_lst	*home_node;
	size_t		key_len;
	char		*current_path;
	char		*home_path;
	size_t		new_path_len;

	pwd_node = ms_lst_get_node_with_the_same_key(envp_lst, "PWD=");
	home_node = ms_lst_get_node_with_the_same_key(envp_lst, "HOME=");
	key_len = 4;
	current_path = pwd_node->entry + key_len;
	home_path = home_node->entry + key_len + 1;
	if (ms_strcmp(cmd_prompt->prompt, current_path) == MS_SAME)
		return ;
	new_path_len = ms_get_new_path_length(\
		home_path, current_path, cmd_prompt->home_path_len \
	);
	printf("new_path_len: %d, home: %d\n",new_path_len, cmd_prompt->home_path_len);
	ms_generate_new_path_for_prompt(env, current_path, new_path_len, false);
}
