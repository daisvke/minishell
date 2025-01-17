/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1_cd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 10:07:43 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/09 21:13:09 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_print_cd_err_messages(int err_code, char *path)
{
	if (err_code == 1)
	{
		ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
		ppx_putstr_fd(path, STDERR_FILENO, MS_NONE);
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	}
	else
	{
		ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
		write(STDERR_FILENO, "cd: too many arguments\n", 24);
	}
}

void	ms_set_current_pwd_to_oldpwd(t_ms *env)
{
	t_env_lst	*node_pwd;
	t_env_lst	*node_oldpwd;
	char		*current_pwd;
	char		*new_path;

	node_pwd = ms_lst_get_node_with_the_same_key(env->envp_lst, "PWD=");
	node_oldpwd = ms_lst_get_node_with_the_same_key(env->envp_lst, "OLDPWD=");
	if (node_pwd && node_oldpwd)
	{
		current_pwd = node_pwd->entry;
		new_path = ppx_join_three_str(\
			env, "OLDPWD", "=", &current_pwd[4] \
		);
		ms_lst_assign_entry_to_node(node_oldpwd, new_path);
		new_path = ms_free(new_path);
	}
}

void	ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *arg_path)
{
	char		*path;
	bool		malloced;

	malloced = false;
	if (arg_path == NULL || arg_path[0] == 0)
	{
		path = ms_get_home_value_from_envp_lst(ms_env);
		malloced = true;
	}
	else
		path = arg_path;
	if (ms_check_if_there_is_not_too_much_args(ppx_env->cmd) == MS_OK)
	{
		if (ppx_check_access(path, PPX_OFF) == MS_SUCCESS)
		{
			ms_set_current_pwd_to_oldpwd(ms_env);
			chdir(path);
		} else
			ms_print_cd_err_messages(1, path);
		ms_update_prompt_when_home_is_unset(ms_env, false);
	}
	else
		ms_print_cd_err_messages(2, path);
	if (malloced == true)
		path = ms_free(path);
}
