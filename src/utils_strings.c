/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 09:21:17 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/14 06:41:57 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	ms_check_if_char_is_a_redir_symbol(int c)
{
	char	*symbols;

	if (!(char)c)
		return ('\0');
	symbols = "<>";
	while (*symbols)
	{
		if ((char)c == *symbols)
			return (*symbols);
		symbols++;
	}
	return ('\0');
}

char	*ms_search_redir_symbol(char *str)
{
	char	symbol;
	
	if (!str)
		return (NULL);
	if (ms_check_if_char_is_a_redir_symbol(*str))
	{
		symbol = ms_check_if_char_is_a_redir_symbol(*str);
		if (*(str + 1) == symbol)
			return (str + 2);
		return (str + 1);
	}
	return (0);
}
