/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_and_activate_options.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 03:44:36 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/28 05:29:43 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ms_free_split(char *split[])
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

char	*ms_strdup(char *src, size_t size)
{
	size_t	i;
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

int	ms_split_iter(char *split[], char *s, char c)
{
	size_t	i;
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
		split[i] = ms_strdup(start, s - start);
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

void	ms_activate_option(t_ms *env, int sep)
{
	if ((char)sep == '|')
		env->options |= MS_OPT_PIPE;
//	if ((char)sep == '<<')
//		env->options |=
}

size_t	ms_wordcount(t_ms *env, char *s, int sep)
{
	size_t	wc;

	wc = 0;
	while (*s)
	{
		while (*s == (char)sep)
		{
			ms_activate_option(env, sep);
			s++;
		}
		if (!*s)
			break ;
		while (*s != (char)sep && *s)
			s++;
		wc++;
	}
	return (wc);
}

char	**ms_split_and_activate_options(t_ms *env, char const *s, char c)
{
	char	**split;
	int		res;
	size_t	wordcount;

	wordcount = ms_wordcount(env, (char *)s, c);
	split = (char **)malloc(sizeof(*split) * (wordcount + 1));
	if (!split)
		return (NULL);
	res = ms_split_iter(split, (char *)s, c);
	if (res == ERROR)
		return (NULL);
	return (split);
}
