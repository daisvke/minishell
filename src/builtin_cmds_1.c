/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 02:31:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/08 02:37:48 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ms_check_if_there_is_not_too_much_args(char **cmd_and_args)
{
	size_t	i;

	i = 0;
	while (cmd_and_args[i])
	{
		if (i > 1)
			return (MS_ERROR); //error
		++i;
	}
	return (MS_OK);
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
			ppx_exit_with_error_message(ppx_env, 10);
		node = ms_lst_get_node_with_the_same_key(ms_env->envp_lst, "PWD=");
		new_path = ppx_join_three_str(ppx_env, "PWD", "=", current_absolute_path);
		ms_lst_assign_entry_to_node(node, new_path);
		new_path = ms_free(new_path);
	}
	else
		write(STDOUT_FILENO, "cd: Too many arguments\n", 24);
}

void	ms_execute_cmd_pwd(t_env_lst *envp_lst)
{
	size_t		key_len;
	t_env_lst	*node;
	char		*current_path;

	node = ms_lst_get_node_with_the_same_key(envp_lst, "PWD=");
	key_len = 4;
	current_path = node->entry + key_len; 
	printf("%s\n", current_path);
}

void	ms_execute_cmd_env(t_env_lst *envp_head)
{
	t_env_lst *node;

	node = envp_head;
	while (node)
	{
		printf("%s\n", node->entry);
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
			printf(" ");
		++i;
	}
	if (opt_n == false)
		printf("\n");
}

