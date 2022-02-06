/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1_cd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 10:07:43 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/06 11:36:08 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_get_oldpwd_value_from_envp_lst(t_ms *env) // =>have to update oldpwd when cd
{
	char		*oldpwd_path;
	t_env_lst	*oldpwd_node;
	size_t		key_len;
	
	oldpwd_node = ms_lst_get_node_with_the_same_key(env->envp_lst, "OLDPWD=");
	if (oldpwd_node == NULL)
		oldpwd_path = getcwd(NULL, 0);
	else
	{
		key_len = 7;
		oldpwd_path = ms_strdup(oldpwd_node->entry + 7, \
			ms_strlen(oldpwd_node->entry) - 7);//protect?
	}
	return (oldpwd_path); 
}

void	ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *arg_path)
{
	char		*current_absolute_path;
	t_env_lst	*node;
	char		*path;
	char		*new_path;
	bool		malloced;

	malloced = false;
	if (arg_path == NULL || arg_path[0] == 0)
	{
		path = ms_get_home_value_from_envp_lst(ms_env);
		malloced = true;
	}
	else if (ms_strcmp(arg_path, "-") == MS_SAME)
	{
		path = ms_get_oldpwd_value_from_envp_lst(ms_env);//init: oldpwd= NULL?
		malloced = true;
	}
	else
		path = arg_path;
	if (ms_check_if_there_is_not_too_much_args(ppx_env->cmd) == MS_OK)
	{
		if (ppx_check_access(path, PPX_OFF) == MS_SUCCESS)
			chdir(path);
		else
		{
			ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
			ppx_putstr_fd(path, STDERR_FILENO, MS_NONE);
			write(STDERR_FILENO, ": No such file or directory\n", 28);
		}
		current_absolute_path = getcwd(NULL, 0);
		if (current_absolute_path == NULL)
			ms_exit_with_error_message(ms_env, 5);
		node = ms_lst_get_node_with_the_same_key(ms_env->envp_lst, "PWD=");
		new_path = ppx_join_three_str(\
			ppx_env, "PWD", "=", current_absolute_path \
		);
		current_absolute_path = ms_free(current_absolute_path);
		ms_lst_assign_entry_to_node(ms_env, node, new_path);
		new_path = ms_free(new_path);
		ms_get_new_path_for_prompt(\
			ms_env, ms_env->envp_lst, &ms_env->cmd_prompt);
	}
	else
	{
		ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
		write(STDERR_FILENO, "cd: too many arguments\n", 24);
	}
	if (malloced == true)
		path = ms_free(path);
}
