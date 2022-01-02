/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/24 03:45:01 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ppx_free_split(char *split[])
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		++i;
	}
	free(split);
	split = NULL;
}

char	*ppx_strdup(char *src, int size)
{
	int		i;
	char	*dest;

	dest = (char *)malloc(sizeof(*dest) * (size + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = '\0';
	return (dest);
}

int	ppx_split_iter(char *split[], char *str, char sep)
{
	int		i;
	char	*start;
	bool	quotes;

	quotes = false;
	i = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (!*str)
			break ;
		start = str;
		if (*str == '\'' || *str == '\"')
		{
			quotes = true;
			str += ms_handle_quotes(str, *str);
		}
		while (*str && *str != (char)sep && *str != '\'' && *str != '\"')
			str++;
		split[i] = ppx_strdup(start + quotes, str - start - (2 * quotes));
		if (!split[i])
		{
			ppx_free_array_of_pointers(split, i);
			return (ERROR);
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

	split = (char **)malloc(sizeof(*split) * (ppx_wordcount((char *)s, sep) + 1));
	if (!split)
		return (NULL);
	res = ppx_split_iter(split, (char *)s, sep);
	if (res == ERROR)
		return (NULL);
	return (split);
}
