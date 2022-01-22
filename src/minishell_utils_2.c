/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 00:01:41 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/22 20:19:00 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_use_argv_to_create_cmd_line(int argc, char *argv[], t_ms *env)
{
	int		i;
	size_t	j;

	env->split_cmd_line = ms_malloc(env, argc, sizeof(char *));
	i = MS_FIRST_ARG_POS;
	j = 0;
	while (i < argc)
	{
		env->split_cmd_line[j] = ms_strdup(argv[i], ms_strlen(argv[i]));
		if (env->split_cmd_line[j] == NULL)
		{
			ppx_free_array_of_pointers(&env->split_cmd_line, j);
			ms_exit_with_error_message(env, 0);
		}
		++j;
		++i;
	}
	env->split_cmd_line[j] = NULL;
}

int	ms_check_arguments(char *envp[], int argc)
{
	if (envp == NULL)
	{
		ms_print_error_message(17);
		exit(EXIT_FAILURE);
	}
	if (argc < 1)
	{
		ms_print_error_message(18);
		exit(EXIT_FAILURE);
	}
	return (MS_OK);
}

bool	ms_detect_overuse_of_consecutive_symbol(char symbol, size_t i)
{
	return ((symbol == '|' && i > 1) \
		|| (symbol == '<' && i > 3) \
		|| (symbol == '>' && i > 2));
}

int	ms_check_pipes_and_redirections(t_ms *env, char *cmd_line)
{
	size_t	len;
	char	symbol;
	size_t	i;

	len = ms_strlen(env->cmd_line);
	if (env->cmd_line[0] == '|' \
		|| (len > 0 && env->cmd_line[len - 1] == '|'))
		return (2);
	while (cmd_line && *cmd_line)
	{
		if (*cmd_line == '|' || *cmd_line == '<' || *cmd_line == '>')
		{
			i = 0;
			symbol = *cmd_line;
			while (cmd_line && *cmd_line == symbol)
			{
				cmd_line++;
				++i;
			}
			if (ms_detect_overuse_of_consecutive_symbol(symbol, i) == true)
				return (ms_get_symbol_error_code(symbol));
		}
		if (*cmd_line)
			cmd_line++;
	}
	if (*(cmd_line- 1) == '<' || *(cmd_line - 1) == '>')
		return (19);
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
