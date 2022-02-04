/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/04 08:18:51 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ppx_split_iter(char *split[], char *str, t_split opt)
{
	int	get_start;
	int	len;

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
		get_start = opt.quotes - opt.first_char_not_quote;
		if (get_start < 0)
			get_start = 0;
		len = str - opt.start - (2 * (opt.quotes));
		if (len < 0)
			len = 0;
		split[opt.i] = ppx_strdup(opt.start + get_start, len);
		if (!split[opt.i])
		{
			ppx_free_array_of_pointers(&split, opt.i);
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
	size_t	len;
	char	**split;
	t_split	opt;

	len = ppx_wordcount((char *)str, sep) + 1;
	split = malloc(sizeof(char *) * len);
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
