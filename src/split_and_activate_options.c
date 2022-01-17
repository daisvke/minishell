/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_and_activate_options.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 03:44:36 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/17 04:06:12 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ms_handle_quotes(void *env, char *str, char quote)
{
	size_t	i;

	i = 1;
	while (str[i] && str[i] != quote)
		++i;
	return (i + 1);
}

int	ms_split_iter(t_ms *env, char *split[], char *str, char sep)
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
			str += ms_handle_quotes(env, str, *str);
		while (*str && *str != sep)
			str++;
		split[i] = ms_strdup(start, str - start);
		if (!split[i])
		{
			ppx_free_array_of_pointers(split, i);
			return (MS_ERROR);
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
}

size_t	ms_wordcount(t_ms *env, char *str, int sep)
{
	size_t		wc;

	wc = 0;
	while (*str)
	{
		while (*str == (char)sep)
		{
			ms_activate_option(env, sep);
			str++;
		}
		if (!*str)
			break ;
		if (*str == '\'' || *str == '\"')
			str += ms_handle_quotes(env, str, *str);
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
	res = ms_split_iter(env, split, (char *)s, sep);
	if (res == MS_ERROR)
		return (NULL);
	int i;
	for (i=0;split[i];++i)
		printf("split_active: %s\n",split[i]);
	return (split);
}
