/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 02:57:29 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/24 03:00:36 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ms_found_last_pipe_exit_status_symbols(char *cmd_line, t_expv *vars)
{
	if (cmd_line[vars->i] == '$' && cmd_line[vars->i + 1] == '?')
	{
		vars->found_var = true;
		return (true);
	}
	return (false);
}

void	ms_get_last_pipe_exit_status_length(t_ms *env, t_expv *vars)
{
	size_t	len;

	len = ms_nbrlen(env->last_pipe_exit_status);
	vars->len += len - 1;
}

void	ms_expand_last_pipe_exit_status(\
	t_ms *env, char *new_cmd_line, t_expv *vars)
{
	char	*exit_status;

	exit_status = ms_itoa(env, env->last_pipe_exit_status);
	ms_copy_value_to_the_expansion_location(exit_status, new_cmd_line, vars);
	exit_status = ms_free(exit_status);
	++vars->i;
}