/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1_cd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 10:07:43 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/07 04:00:59 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ms_update_prompt_when_home_is_unset(ms_env);
	}
	else
	{
		ppx_putstr_fd("minishell: ", STDERR_FILENO, MS_NONE);
		write(STDERR_FILENO, "cd: too many arguments\n", 24);
	}
	if (malloced == true)
		path = ms_free(path);
}
