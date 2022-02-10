/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 02:03:33 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/09 21:14:19 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ppx_strdup(char *src, size_t size)
{
	size_t		i;
	size_t		j;
	char		*dest;

	dest = (char *)malloc(sizeof(*dest) * (size + 1));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	while (i < size)
	{
		while (src[j] == '\'' || src[j] == '\"')
			++j;
		dest[i] = src[j];
		++i;
		++j;
	}
	dest[i] = '\0';
	return (dest);
}

int	ms_free_split_and_return_error(char *split[], t_split opt)
{
	ppx_free_array_of_pointers(&split, opt.i);
	return (MS_ERROR);
}

int	ppx_split_iter(char *split[], char *str, t_split opt, int len)
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
		len = str - opt.start - (2 * (opt.quotes));
		if (len < 0)
			len = 0;
		split[opt.i] = ppx_strdup(opt.start, len);
		if (!split[opt.i])
			ms_free_split_and_return_error(split, opt);
		opt.first_char_not_quote = false;
		++opt.i;
	}
	split[opt.i] = 0;
	return (0);
}

char	**ppx_split(char const *str, char sep)
{
	int		res;
	int		len;
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
	len = 0;
	res = ppx_split_iter(split, (char *)str, opt, len);
	if (res == MS_ERROR)
		return (NULL);
	return (split);
}
