/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 02:31:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/30 09:44:19 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ms_check_if_the_cmd_is_implemented(\
	char **cmd_line, size_t *cmd_code, bool process)
{
	*cmd_code = 0;
	if (process == PPX_PROC_PARENT)
	{
		if (ms_strcmp(cmd_line[0], "cd") == MS_SAME)
			*cmd_code = MS_CMD_CD;
		else if (ms_strcmp(cmd_line[0], "exit") == MS_SAME)
			*cmd_code = MS_CMD_EXIT;
		else if (ms_strcmp(cmd_line[0], "export") == MS_SAME)
			*cmd_code = MS_CMD_EXPORT;
		else if (ms_strcmp(cmd_line[0], "unset") == MS_SAME)
			*cmd_code = MS_CMD_UNSET;
	}
	else if (process == PPX_PROC_CHILD)
	{
		if (ms_strcmp(cmd_line[0], "pwd") == MS_SAME)
			*cmd_code = MS_CMD_PWD;
		else if (ms_strcmp(cmd_line[0], "echo") == MS_SAME)
			*cmd_code = MS_CMD_ECHO;
		else if (ms_strcmp(cmd_line[0], "env") == MS_SAME)
			*cmd_code = MS_CMD_ENV;
	}
	return (*cmd_code);
}

void	ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *path)
{
	char		*current_absolute_path;
	t_env_lst	*node;
	char		*new_path;

	if (ms_check_if_there_is_not_too_much_args(ppx_env->cmd) == MS_OK)
	{
		chdir(path);
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
	}
	else
		write(STDERR_FILENO, "cd: too many arguments\n", 24);
}

void	ms_execute_cmd_pwd(t_env_lst *envp_lst)
{
	size_t		key_len;
	t_env_lst	*node;
	char		*current_path;

	node = ms_lst_get_node_with_the_same_key(envp_lst, "PWD=");
	key_len = 4;
	current_path = node->entry + key_len;
	write(STDOUT_FILENO, current_path, ms_strlen(current_path));
	write(STDOUT_FILENO, "\n", 1);
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

void	ms_execute_cmd_echo(char *cmd[])
{
	bool	opt_n;	
	size_t	i;

	opt_n = false;
	if (ms_strcmp(cmd[1], "-n") == MS_SAME)
		opt_n = true;
	i = MS_FIRST_ARG_POS + opt_n;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1] != NULL)
			write(STDOUT_FILENO, MS_SPACE_TO_SEP_PRINTED_ARGS, 1);
		++i;
	}
	if (opt_n == false)
		write(STDOUT_FILENO, "\n", 1);
}
