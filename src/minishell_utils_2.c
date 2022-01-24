/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 00:01:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/24 02:49:04 by dtanigaw         ###   ########.fr       */
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

size_t	ms_detect_overuse_of_consecutive_symbol(char *cmd_line)
{
	char	symbol;
	size_t	i;

	i = 0;
	symbol = cmd_line[i];
	while (cmd_line && cmd_line[i] == symbol)
		++i;
	if ((symbol == '|' && i > 1) \
		|| (symbol == '<' && i > 3) \
		|| (symbol == '>' && i > 2))
		return (ms_get_symbol_error_code(symbol));
	return (0);
}

int	ms_check_pipes_and_redirections(t_ms *env, char *cmd_line)
{
	size_t	len;
	size_t	res;

	len = ms_strlen(env->cmd_line);
	if (env->cmd_line[0] == '|' \
		|| (len > 0 && env->cmd_line[len - 1] == '|'))
		return (2);
	while (cmd_line && *cmd_line)
	{
		if (*cmd_line == '|' || *cmd_line == '<' || *cmd_line == '>')
		{
			res = ms_detect_overuse_of_consecutive_symbol(cmd_line);
			if (res > 1)
				return (res);
		}
		if (*cmd_line)
			cmd_line++;
	}
	while (len > 1 && (*cmd_line == ' ' || *cmd_line == '\0'))
		cmd_line--;
	if (*(cmd_line) == '|')
		return (2);
	if (*(cmd_line) == '<' || *(cmd_line) == '>')
		return (3);
	return (MS_SUCCESS);
}

int	ms_check_if_quote_nbr_is_even(char *cmd_line)
{
	size_t	count_sgl;
	size_t	count_dbl;

	count_sgl = 0;
	count_dbl = 0;
	while (cmd_line && *cmd_line)
	{
		if (*cmd_line == '\'')
			++count_sgl;
		else if (*cmd_line == '\"')
			++count_dbl;
		cmd_line++;
	}
	if (count_sgl % 2 != MS_EVEN || count_dbl % 2 != MS_EVEN)
		return (15);
	return (MS_SUCCESS);
}
