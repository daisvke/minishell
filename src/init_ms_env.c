/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ms_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 10:19:54 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 11:41:22 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_set_environment_variables(char *envp[], t_ms *env)
{
	size_t	i;
	size_t	max;
	size_t	len;

	i = 0;
	while (envp[i])
		++i;
	max = i;
	env->envp = malloc(sizeof(char *) * (max + 1));
	i = 0;
	while (i < max)
	{
		//ppx?
		len = ppx_strlen(envp[i]) + 1;
		env->envp[i] = malloc(sizeof(char) * len);
		//ppx?
		ppx_memcpy(env->envp[i], envp[i], len);
		++i;
	}
	env->envp[i] = NULL;
}

void	ms_convert_envp_into_linked_list(char *envp[], t_ms *env)
{
	size_t	i;
	size_t	max;
	size_t	len;

	i = 0;
	while (envp[i])
		++i;
	max = i;
	env->envp_lst = malloc(sizeof(t_env_lst));
	i = 0;
	while (i < max)
	{
		//ppx?
		len = ppx_strlen(envp[i]) + 1;
		env->envp[i] = malloc(sizeof(char) * len);
		//ppx?
		ppx_memcpy(env->envp[i], envp[i], len);
		++i;
	}
	env->envp[i] = NULL;
}

void	ms_init_env(char *envp[], t_ms *env)
{
	char	*current_absolute_path;

	ft_memset(env, 0, sizeof(t_ms));
	ms_set_environment_variables(envp, env);
	ms_convert_envp_into_linked_list(envp, env);
//	if (curr_abs_path == NULL)
//		ppx_exit_with_error_message(env, 10);
}
