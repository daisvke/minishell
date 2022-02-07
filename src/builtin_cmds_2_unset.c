/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_2_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 09:02:30 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/07 12:06:31 by dtanigaw         ###   ########.fr       */
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

void	ms_update_prompt_when_home_is_unset(t_ms *env, bool first_time)
{
	t_env_lst	*node;
	char		*current_absolute_path;
	char		*new_path;

	current_absolute_path = getcwd(NULL, 0);
	if (current_absolute_path == NULL)
		ms_exit_with_error_message(env, 5);
	node = ms_lst_get_node_with_the_same_key(env->envp_lst, "PWD=");
	new_path = ppx_join_three_str(\
		&env->ppx_env, "PWD", "=", current_absolute_path \
	);
	current_absolute_path = ms_free(current_absolute_path);
	ms_lst_assign_entry_to_node(env, node, new_path);
	new_path = ms_free(new_path);
	ms_get_new_path_for_prompt(\
		env, env->envp_lst, &env->cmd_prompt, first_time);
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
		if (ms_strcmp(cmdline[i], "HOME") == MS_SAME)
			ms_update_prompt_when_home_is_unset(env, false);
		++i;
	}
}
