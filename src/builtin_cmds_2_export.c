/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_2_export.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 09:02:37 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 09:34:15 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_check_character_2(char *cmd[], size_t i, size_t j)
{
	if (j == 0 || cmd[i][j - 1] == ' ')
	{
		printf(\
		"minishell: export: `%s' : not a valid identifier\n", cmd[i]);
		return (MS_ERROR);
	}
	return (MS_SUCCESS);
}

int	ms_check_character_1(char *cmd[], size_t i, size_t j)
{
	if ((j == 0 && ms_isdigit(cmd[i][j]) == true) \
		|| (ms_isalnum(cmd[i][j]) == false && cmd[i][j] != '_'))
	{
		printf(\
		"minishell: export: `%s' : not a valid identifier\n", cmd[i]);
		return (MS_ERROR);
	}
	return (MS_SUCCESS);
}

int	ms_check_export_args(t_ms *env, char *cmd[])
{
	size_t	i;
	size_t	j;

	i = MS_FIRST_ARG_POS;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (ms_check_character_1(cmd, i, j) == MS_ERROR)
				return (MS_ERROR);
			++j;
		}
		if (cmd[i][j] == '=')
		{
			if (ms_check_character_2(cmd, i, j) == MS_ERROR)
				return (MS_ERROR);
		}
		else
			return (MS_NO_EXPORT);
		++i;
	}
	return (0);
}

int	ms_create_entry_if_not_already_existing(\
	t_ms *env, t_env_lst *node, char *cmd_line[], size_t i)
{
	t_env_lst	*new;

	if (ms_compare_with_envp_key(node->entry, cmd_line[i], true) == MS_SAME)
	{
		ms_lst_assign_entry_to_node(env, node, cmd_line[i]);
		return (MS_FOUND);
	}
	else if (node->next == NULL)
	{
		new = ms_lst_create_new_node(env, cmd_line[i]);
		ms_lst_add_back(env->envp_lst, new);
	}
	return (MS_NOT_FOUND);
}

void	ms_execute_cmd_export(t_ms *env, char *cmd_line[])
{
	size_t		i;
	t_env_lst	*node;

	if (ms_check_export_args(env, cmd_line) == MS_NO_EXPORT)
		return ;
	i = MS_FIRST_ARG_POS;
	while (cmd_line[i])
	{
		node = env->envp_lst;
		while (node)
		{
			if (ms_create_entry_if_not_already_existing(\
				env, node, cmd_line, i) == MS_FOUND)
				break ;
			node = node->next;
		}
		++i;
	}
}
