/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/16 10:07:25 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_free_split(char *split[])
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		split[i] = ms_free(split[i]);
		++i;
	}
	split = ms_free(split);
}

int	ppx_check_quotes(char *str, char c, bool *quotes)
{
	if (c == '\'' || c == '\"')
	{
		*quotes = true;
		return (ms_handle_quotes(str, c));
	}
}

int	ppx_split_iter(char *split[], char *str, char sep, bool *quotes)
{
	int		i;
	char	*start;

	i = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (!*str)
			break ;
		start = str;
		str += ppx_check_quotes(str, *str, quotes);
		while (*str && *str != (char)sep && *str != '\'' && *str != '\"')
			str++;
		split[i] = ppx_strdup(start + *quotes, str - start - (2 * *quotes));
		if (!split[i])
		{
			ppx_free_array_of_pointers(split, i);
			return (MS_ERROR);
		}
		++i;
	}
	split[i] = 0;
	return (0);
}

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

char	**ppx_split(char const *s, char sep)
{
	int		res;
	char	**split;
	bool	quotes;

	split = (\
		char **)malloc(sizeof(*split) * (ppx_wordcount((char *)s, sep) + 1) \
	);
	if (!split)
		return (NULL);
	quotes = false;
	res = ppx_split_iter(split, (char *)s, sep, &quotes);
	if (res == MS_ERROR)
		return (NULL);
	return (split);
}
