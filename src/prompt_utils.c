/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 05:06:40 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/10 05:11:20 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_print_only_tilde_and_dollar_when_at_home(t_ms *env)
{
	env->cmd_prompt.prompt = ms_strdup(\
		"~\001\x1B[2m\x1B[37m\002$ ", 3 + 11);
	if (env->cmd_prompt.prompt == NULL)
		exit(EXIT_FAILURE);
}

void	ms_print_only_slash_and_dollar_when_before_home(t_ms *env)
{
	env->cmd_prompt.prompt = ppx_join_three_str(\
		&env->ppx_env, \
		"/", "\001\x1B[2m\x1B[37m\002$", " ");
}
