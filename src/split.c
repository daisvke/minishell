/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/17 22:34:11 by dtanigaw         ###   ########.fr       */
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

int	ppx_split_iter(char *split[], char *str, t_split opt)
{
	while (*str)
	{
		while (*str == opt.sep)
			str++;
		if (!*str)
			break ;
		opt.start = str;
		opt.quotes = false;
		while (*str && *str != opt.sep && *str != '\'' && *str != '\"')
			str++;
		str += ppx_check_quotes(str, *str, &opt.quotes);
		while (*str && *str != opt.sep && *str != '\'' && *str != '\"')
			str++;
		split[opt.i] = ppx_strdup(opt.start + opt.quotes - \
			opt.first_char_not_quote, str - opt.start - (2 * (opt.quotes)));
		if (!split[opt.i])
		{
			ppx_free_array_of_pointers(split, opt.i);
			return (MS_ERROR);
		}
		opt.first_char_not_quote = false;
		++opt.i;
	}
	split[opt.i] = 0;
	return (0);
}

char	**ppx_split(char const *str, char sep)
{
	int		res;
	char	**split;
	t_split	opt;

	split = (\
		char **)malloc(sizeof(*split) * (ppx_wordcount((char *)str, sep) + 1) \
	);
	if (!split)
		return (NULL);
	opt.i = 0;
	opt.sep = sep;
	opt.quotes = ppx_check_if_str_contains_quotes((char *)str);
	opt.first_char_not_quote = false;
	if (opt.quotes == true && (*str != '\'' && *str != '\"'))
		opt.first_char_not_quote = true;
	res = ppx_split_iter(split, (char *)str, opt);
	if (res == MS_ERROR)
		return (NULL);
	return (split);
}
