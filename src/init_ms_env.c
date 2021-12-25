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

void	ms_init_env(t_ms *env)
{
	char	*current_absolute_path;

	ft_memset(env, 0, sizeof(t_ms));
	current_absolute_path = getcwd(NULL, 0);
//	if (curr_abs_path == NULL)
//		ppx_exit_with_error_message(env, 10);
	env->current_directory = current_absolute_path;
}
