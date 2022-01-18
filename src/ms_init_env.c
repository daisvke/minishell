/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 10:19:54 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 10:00:17 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_convert_envp_into_linked_list(char *envp[], t_ms *env)
{
	size_t		i;
	size_t		max;
	t_env_lst	*new;

	i = 0;
	while (envp[i])
		++i;
	max = i;
	env->envp_lst = ms_lst_create_new_node(env, envp[0]);
	i = 1;
	while (i < max)
	{
		new = ms_lst_create_new_node(env, envp[i]);
		ms_lst_add_back(env->envp_lst, new);
		++i;
	}
}

void	ms_init_env(char *envp[], t_ms *env, bool create_envp_lst)
{
	ms_memset(env, 0, sizeof(t_ms));
	if (create_envp_lst)
		ms_convert_envp_into_linked_list(envp, env);
}
