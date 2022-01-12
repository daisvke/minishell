/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  builtin_cmds_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 02:34:26 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/08 05:31:56 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if ((j == 0 && ms_isdigit(cmd[i][j]) == true) \
				|| (ms_isalnum(cmd[i][j]) == false && cmd[i][j] != '_'))
			{
				printf("minishell: export: `%s' : not a valid identifier\n", cmd[i]);
				return (MS_ERROR);
			}
			++j;
		}
		if (cmd[i][j] == '=')
		{
			if (j == 0 || cmd[i][j - 1] == ' ')
			{
				printf("minishell: export: `%s' : not a valid identifier\n", cmd[i]);
				return (MS_ERROR);
			}
		}
		else
			return (MS_NO_EXPORT);
		++i;
	}
	return (0);
}

void    ms_execute_cmd_export(t_ms *env, char *cmd_line[])
{
    size_t  i;
	t_env_lst   *node;
	t_env_lst   *new;

	if (ms_check_export_args(env, cmd_line) == MS_NO_EXPORT \
		|| ms_check_export_args(env, cmd_line) == MS_ERROR)
		return ;
	i = MS_FIRST_ARG_POS;
	while (cmd_line[i])
	{
        node = env->envp_lst;
		while (node)
		{
			if (ms_compare_with_envp_key(node->entry, cmd_line[i], true) == MS_SAME)
			{
				ms_lst_assign_entry_to_node(node, cmd_line[i]);
				break ;
			}
			else if (node->next == NULL)
			{
				new = ms_lst_create_new_node(cmd_line[i]);
				ms_lst_add_back(env->envp_lst, new);
			}
			node = node->next;
		}
		++i;
	}
}

int	ms_check_unset_args(t_ms *env, char *cmd[])
{
	size_t	i;
	size_t	j;

	i = MS_FIRST_ARG_POS;
	while (cmd[i])
	{ 
		j = 0;
		while (cmd[i][j])
		{
			if ((j == 0 && ms_isdigit(cmd[i][j]) == true) \
				|| (ms_isalnum(cmd[i][j]) == false && cmd[i][j] != '_'))
			{
				printf("minishell: unset: `%s' : not a valid identifier\n", cmd[i]);
				return (MS_ERROR);
			}
			++j;
		}
		++i;
	}
	return (MS_SUCCESS);
}

void    ms_execute_cmd_unset(t_ms *env, char *cmd_line[])
{
    size_t  i;
	t_env_lst   *node;
	t_env_lst	*tmp_before;
	t_env_lst	*tmp_next;

	if (ms_check_unset_args(env, cmd_line) == MS_ERROR)
		return ;
	i = MS_FIRST_ARG_POS;
	while (cmd_line[i])
	{
        node = env->envp_lst;
		while (node)
		{
			if (node->next && ms_compare_with_envp_key(((t_env_lst *)node->next)->entry, cmd_line[i], false) == MS_SAME)
			{
				tmp_before = node;
				node = node->next;
				tmp_next = node->next;
				tmp_before->next = tmp_next;
				ms_lst_del_node(node);
				break ;
			}
			node = node->next;
		}
		++i;
	}
}
