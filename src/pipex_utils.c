/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 04:00:34 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/19 13:46:32 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ms_check_if_the_cmd_is_implemented(\
	char **cmdline, size_t *cmd_code, bool process)
{
	*cmd_code = 0;
	if (process == PPX_PROC_PARENT || process == PPX_PROC_CHILD)
	{
		if (ms_strcmp(cmdline[0], "cd") == MS_SAME)
			*cmd_code = MS_CMD_CD;
		else if (ms_strcmp(cmdline[0], "exit") == MS_SAME)
			*cmd_code = MS_CMD_EXIT;
		else if (ms_strcmp(cmdline[0], "export") == MS_SAME)
			*cmd_code = MS_CMD_EXPORT;
		else if (ms_strcmp(cmdline[0], "unset") == MS_SAME)
			*cmd_code = MS_CMD_UNSET;
	}
	if (process == PPX_PROC_CHILD)
	{
		if (ms_strcmp(cmdline[0], "pwd") == MS_SAME)
			*cmd_code = MS_CMD_PWD;
		else if (ms_strcmp(cmdline[0], "echo") == MS_SAME)
			*cmd_code = MS_CMD_ECHO;
		else if (ms_strcmp(cmdline[0], "env") == MS_SAME)
			*cmd_code = MS_CMD_ENV;
	}
	return (*cmd_code);
}

int	ppx_create_array_of_commands(t_ms *ms_env, t_ppx *ppx_env, char *cmdline[])
{
	ppx_env->cmd = ppx_split(cmdline[ppx_env->i], ' ');
	if (!ppx_env->cmd)
		ms_exit_with_error_message(ms_env, 7);
	if (!*ppx_env->cmd || !**ppx_env->cmd)
		return (MS_READ_NONE);
	return (MS_SUCCESS);
}

bool	ms_pipe_is_off_and_cmd_is_implemented(t_ppx *env, size_t *cmd_code)
{
	return ((env->options & MS_OPT_PIPE) == false \
		&& ms_check_if_the_cmd_is_implemented(\
			env->cmd, cmd_code, PPX_PROC_PARENT) == true);
}
