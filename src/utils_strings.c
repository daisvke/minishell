/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 09:21:17 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/10 09:47:00 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)(*s1) - (unsigned char)(*s2));
		s1++;
		s2++;
	}
	return (0);
}

int	ms_strncmp(const char *s1, const char *s2, size_t n)
{
	while ((*s1 || *s2) && n--)
	{
		if (*s1 != *s2)
			return ((unsigned char)(*s1) - (unsigned char)(*s2));
		s1++;
		s2++;
	}
	return (0);
}

void	ms_get_array_of_redirection_symbols(char *array[])
{
	array[0] = "<";
	array[1] = ">";
	array[2] = "<<";
	array[3] = ">>";
	array[4] = NULL;
}

char	ms_check_if_char_is_a_redir_symbol(int c)
{
	char	*symbols;

	if (!(char)c)
		return ('\0');
	symbols = "<>";
	while (*symbols)
	{
		if ((char)c == *symbols)
			return (*symbols);
		symbols++;
	}
	return ('\0');
}

char	*ms_search_redir_symbol(char *str, int *index)
{
	char	symbol;
	
	if (!str)
		return (NULL);
	if (ms_check_if_char_is_a_redir_symbol(*str))
	{
		symbol = ms_check_if_char_is_a_redir_symbol(*str);
		if (*(str + 1) == symbol)
		{
			if (index)
				*index += 2;
			return (str + 2);
		}
		if (index)
			*index += 1;
		return (str + 1);
	}
	return (0);
}

char	*ms_get_outfile_when_redir_symbol_is_not_at_the_begining(char *str)
{
	char	*file;

	while (*str)
	{
		if (ms_search_redir_symbol(str, NULL))
		{
			file = ms_search_redir_symbol(str, NULL);
			return (file);
		}
		str++;
	}
	return (NULL);
}
