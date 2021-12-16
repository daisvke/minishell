/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/09/27 05:19:19 by dtanigaw         ###   ########.fr       */
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
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ppx_split_iter(char *split[], char *s, char c)
{
	int		i;
	char	*start;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		start = s;
		while (*s != c && *s)
			s++;
		split[i] = ppx_strdup(start, s - start);
		if (!split[i])
		{
			ppx_free_array_of_pointers(split, i);
			return (ERROR);
		}
		i++;
	}
	split[i] = 0;
	return (0);
}

int	ppx_wordcount(char *s, int sep)
{
	int	wc;

	wc = 0;
	while (*s)
	{
		while (*s == (char)sep)
			s++;
		if (!*s)
			break ;
		while (*s != (char)sep && *s)
			s++;
		wc++;
	}
	return (wc);
}

char	**ppx_split(char const *s, char c)
{
	int		res;
	char	**split;

	split = (char **)malloc(sizeof(*split) * (ppx_wordcount((char *)s, c) + 1));
	if (!split)
		return (NULL);
	res = ppx_split_iter(split, (char *)s, c);
	if (res == ERROR)
		return (NULL);
	return (split);
}
