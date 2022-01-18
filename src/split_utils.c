/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 06:09:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 04:56:02 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ppx_wordcount(char *str, int sep)
{
	size_t	wc;

	wc = 0;
	while (*str)
	{
		while (*str == (char)sep)
			str++;
		if (!*str)
			break ;
		if (*str == '\'' || *str == '\"')
			str += ms_handle_quotes(str, *str);
		while (*str && *str != (char)sep && *str != '\'' && *str != '\"')
			str++;
		++wc;
	}
	return (wc);
}

int	ppx_check_quotes(char *str, char c, bool *quotes)
{
	if (c == '\'' || c == '\"')
	{
		*quotes = true;
		return (ms_handle_quotes(str, c));
	}
	return (0);
}

bool	ppx_check_if_str_contains_quotes(char *str)
{
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
			return (true);
		str++;
	}
	return (false);
}
