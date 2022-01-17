/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/17 16:58:53 by dtanigaw         ###   ########.fr       */
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
		return (ms_handle_quotes(NULL, str, c));
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

int	ppx_split_iter(char *split[], char *str, char sep, bool *quotes)
{
	size_t	i;
	char	*start;
	bool	not = false;

	if (*quotes == true && (*str != '\'' && *str != '\"'))
		not = true;
	i = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (!*str)
			break ;
		start = str;
		*quotes = false;
		while (*str && *str != sep && *str != '\'' && *str != '\"')
			str++;
		str += ppx_check_quotes(str, *str, quotes);
		while (*str && *str != sep && *str != '\'' && *str != '\"')
			str++;
		split[i] = ppx_strdup(start + *quotes - not, str - start - (2 * (*quotes)));
		if (!split[i])
		{
			ppx_free_array_of_pointers(split, i);
			return (MS_ERROR);
		}
		not = false;
		++i;
	}
	split[i] = 0;
	return (0);
}

char	**ppx_split(char const *str, char sep)
{
	int		res;
	char	**split;
	bool	quotes;

	split = (\
		char **)malloc(sizeof(*split) * (ppx_wordcount((char *)str, sep) + 1) \
	);
	if (!split)
		return (NULL);
	quotes = ppx_check_if_str_contains_quotes(str);
	res = ppx_split_iter(split, (char *)str, sep, &quotes);
	if (res == MS_ERROR)
		return (NULL);
	return (split);
}
