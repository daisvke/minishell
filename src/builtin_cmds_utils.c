/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:58:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/30 09:54:28 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ms_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

bool	ms_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool	ms_isalnum(int c)
{
	return (ms_isalpha(c) || ms_isdigit(c));
}

bool	ms_check_if_there_is_not_too_much_args(char **cmd_and_args)
{
	size_t	i;

	i = 0;
	while (cmd_and_args[i])
	{
		if (i > 1)
			return (MS_ERROR);
		++i;
	}
	return (MS_OK);
}

void	ms_print_not_valid_identifier_err_message(char *cmd)
{
	write(STDOUT_FILENO, "minishell: export: `", 20);
	write(STDOUT_FILENO, cmd, ms_strlen(cmd));
	write(STDOUT_FILENO, "\' : not a valid identifier\n", 27);
}
