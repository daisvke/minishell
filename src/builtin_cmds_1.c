/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 02:31:28 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/19 13:46:32 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	ms_check_echo_arg(char *arg, bool *opt_n)
{
	if (arg && *arg == '-')
	{
		arg++;
		if (!arg || (arg && *arg != 'n'))
			return (false);
		while (arg && *arg)
		{
			if (*arg != 'n')
				return (false);
			*opt_n = true;
			arg++;
		}
	}
	else
		return (false);
	return (true);
}

void	ms_execute_cmd_echo(char *cmd[])
{
	bool	opt_n;	
	size_t	i;

	opt_n = false;
	i = MS_FIRST_ARG_POS;
	while (cmd[i] && ms_check_echo_arg(cmd[i], &opt_n))
		++i;
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

void	ms_execute_cmd_exit(t_ms *env, char *cmd[])
{
	if ((env->ppx_env.options & MS_OPT_PIPE) == false)
	{
		write(STDERR_FILENO, "exit\n", 5);
		if (cmd[1] != NULL)
		{
			write(STDERR_FILENO, "minishell: exit: ", 17);
			ppx_putstr_fd(cmd[1], STDERR_FILENO, MS_NONE);
			write(STDERR_FILENO, ": numeric argument required\n", 28);
		}
		ms_free_all_allocated_variables(env);
		exit(EXIT_SUCCESS);
	}
}
