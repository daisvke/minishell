/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 00:01:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/04 01:42:57 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_check_arguments(char *envp[], int argc)
{
	if (envp == NULL)
	{
		ms_print_error_message(13);
		exit(EXIT_FAILURE);
	}
	if (argc < 1)
	{
		ms_print_error_message(4);
		exit(EXIT_FAILURE);
	}
	return (MS_OK);
}

size_t	ms_get_symbol_error_code(char symbol)
{
	if (symbol == '|')
		return (2);
	else
		return (3);
}

size_t	ms_detect_overuse_of_consecutive_symbol(char *cmdline)
{
	char	symbol;
	size_t	i;

	i = 0;
	symbol = cmdline[i];
	while (cmdline && cmdline[i] == symbol)
		++i;
	if (symbol == '<' && i == 3)
		return (4);
	if ((symbol == '|' && i > 1) \
		|| (symbol == '<' && i > 3) \
		|| (symbol == '>' && i > 3))
		return (ms_get_symbol_error_code(symbol));
	return (0);
}

int	ms_check_pipes_and_redirections(t_ms *env, char *cmdline)
{
	size_t	len;
	size_t	res;

	len = ms_strlen(env->cmdline);
	if (env->cmdline[0] == '|' \
		|| (len > 0 && env->cmdline[len - 1] == '|'))
		return (2);
	while (cmdline && *cmdline)
	{
		if (*cmdline == '|' || *cmdline == '<' || *cmdline == '>')
		{
			res = ms_detect_overuse_of_consecutive_symbol(cmdline);
			if (res > 1)
				return (res);
		}
		if (*cmdline)
			cmdline++;
	}
	while (len > 1 && (*cmdline == ' ' || *cmdline == '\0'))
		cmdline--;
	if (*cmdline == '|' || *cmdline == '<' || *cmdline == '>')
		return (ms_get_symbol_error_code(*cmdline));
	return (MS_SUCCESS);
}

int	ms_check_if_quote_nbr_is_even(char *cmdline)
{
	size_t	count_sgl;
	size_t	count_dbl;

	count_sgl = 0;
	count_dbl = 0;
	while (cmdline && *cmdline)
	{
		if (*cmdline == '\'')
			++count_sgl;
		else if (*cmdline == '\"')
			++count_dbl;
		cmdline++;
	}
	if (count_sgl % 2 != MS_EVEN || count_dbl % 2 != MS_EVEN)
		return (15);
	return (MS_SUCCESS);
}
