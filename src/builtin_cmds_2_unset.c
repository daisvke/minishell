/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_2_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 09:02:30 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 21:18:28 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_check_unset_args(char *cmd[])
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
				printf(\
				"minishell: unset: `%s' : not a valid identifier\n", cmd[i]);
				return (MS_ERROR);
			}
			++j;
		}
		++i;
	}
	return (MS_SUCCESS);
}

void	ms_del_node_from_lst_and_stick_back_lst(t_env_lst *node)
{
	t_env_lst	*tmp_before;
	t_env_lst	*tmp_next;

	tmp_before = node;
	node = node->next;
	tmp_next = node->next;
	tmp_before->next = tmp_next;
	ms_lst_del_node(node);
}

void	ms_execute_cmd_unset(t_ms *env, char *cmdline[])
{
	size_t		i;
	t_env_lst	*node;

	if (ms_check_unset_args(cmdline) == MS_ERROR)
		return ;
	i = MS_FIRST_ARG_POS;
	while (cmdline[i])
	{
		node = env->envp_lst;
		while (node)
		{
			if (node->next && ms_compare_with_envp_key(\
					((t_env_lst *)node->next)->entry, cmdline[i], false \
				) == MS_SAME)
			{
				ms_del_node_from_lst_and_stick_back_lst(node);
				break ;
			}
			node = node->next;
		}
		++i;
	}
}
