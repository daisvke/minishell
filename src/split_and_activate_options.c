/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_and_activate_options.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 03:44:36 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/29 05:24:14 by dtanigaw         ###   ########.fr       */
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

	dest = malloc(sizeof(char) * (size + 1));
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

size_t	ms_handle_quotes(char *str, char quote)
{
	size_t	i;
	i = 1;
	while (str[i] && str[i] != quote)
		++i;
	if (str[i] == '\0')
		exit(EXIT_FAILURE);
	return (i + 1);
}

int	ms_split_iter(char *split[], char *str, char sep)
{
	size_t	i;
	char	*start;

	i = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (!*str)
			break ;
		start = str;
		if (*str == '\'' || *str == '\"')
			str += ms_handle_quotes(str, *str);
		while (*str && *str != sep)
			str++;
		split[i] = ms_strdup(start, str - start);
		if (!split[i])
		{
			ppx_free_array_of_pointers(split, i);
			return (ERROR);
		}
		i++;
	}
	split[i] = 0;
/*
	for(i=0;split[i];++i)
		printf("split%ld: %s\n", i, split[i]);
*/
	return (0);
}

void	ms_activate_option(t_ms *env, char **str, int sep)
{
	if ((char)sep == '|')
		env->options |= MS_OPT_PIPE;
/*	if ((char)sep == '<' && *(*str + 1) == '<')
	{
		env->options |= MS_OPT_HEREDOC;
		++str;
		printf("str++ <<\n");
	}
//	if ((char)sep == '>>')
		env->options |= MS_OPT_REDIR_OUTPUT;*/
}

size_t	ms_wordcount(t_ms *env, char *str, int sep)
{
	size_t		wc;

	wc = 0;
	while (*str)
	{
		while (*str == (char)sep)
		{
			ms_activate_option(env, &str, sep);
			str++;
		}
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

char	**ms_split_and_activate_options(t_ms *env, char const *s, char sep)
{
	char	**split;
	int		res;
	size_t	wordcount;

	wordcount = ms_wordcount(env, (char *)s, sep);
	split = malloc(sizeof(char *) * (wordcount + 1));
	if (!split)
		return (NULL);
	res = ms_split_iter(split, (char *)s, sep);
	if (res == ERROR)
		return (NULL);
	return (split);
}
