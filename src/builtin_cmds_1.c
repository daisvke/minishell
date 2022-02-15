/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 02:31:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/15 03:28:36 by dtanigaw         ###   ########.fr       */
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

void	ms_execute_cmd_pwd(t_env_lst *envp_lst)
{
	size_t		key_len;
	t_env_lst	*node;
	char		*current_path;

	node = ms_lst_get_node_with_the_same_key(envp_lst, "PWD=");
	if (node == NULL)
	{
		current_path = getcwd(NULL, 0);
		if (current_path == NULL)
			exit(EXIT_FAILURE);
		write(STDOUT_FILENO, current_path, ms_strlen(current_path));
		write(STDOUT_FILENO, "\n", 1);
		current_path = ms_free(current_path);
	}
	else
	{
		key_len = 4;
		current_path = node->entry + key_len;
		write(STDOUT_FILENO, current_path, ms_strlen(current_path));
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	ms_execute_cmd_env(t_env_lst *envp_head)
{
	t_env_lst	*node;

	node = envp_head;
	while (node)
	{
		write(STDOUT_FILENO, node->entry, ms_strlen(node->entry));
		write(STDOUT_FILENO, "\n", 1);
		node = node->next;
	}
}

void	ms_print_if_not_backslash(char *arg)
{
	while (*arg)
	{
		if (*arg != '\\')
			write(STDOUT_FILENO, &*arg, 1);
		arg++;
	}
}

void	ms_execute_cmd_echo(char *cmd[])
{
	bool	opt_n;	
	size_t	i;

	opt_n = false;
	i = MS_FIRST_ARG_POS;
	while (ms_strncmp(cmd[i], "-n", 2) == MS_SAME)
	{
		opt_n = true;
		++i;
	}
	while (cmd[i])
	{
		ms_print_if_not_backslash(cmd[i]);
		if (cmd[i + 1] != NULL)
			write(STDOUT_FILENO, MS_SPACE_TO_SEP_PRINTED_ARGS, 1);
		++i;
	}
	if (opt_n == false)
		write(STDOUT_FILENO, "\n", 1);
}
