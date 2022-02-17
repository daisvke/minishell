/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strings_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 09:21:17 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 04:42:33 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ms_search_redir_symbol(char *str)
{
	char	symbol;

	if (!str)
		return (NULL);
	if (ms_check_if_char_is_a_redir_symbol(*str))
	{
		symbol = ms_check_if_char_is_a_redir_symbol(*str);
		if (*(str + 1) == symbol)
			return (str + 2);
		return (str + 1);
	}
	return (0);
}

size_t	ms_count_char_in_array(char *argv[])
{
	size_t	i;
	size_t	j;
	size_t	res;

	res = 0;
	i = MS_FIRST_ARG_POS;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			++res;
			++j;
		}
		++res;
		++i;
	}
	return (res);
}

char	*ms_convert_array_of_str_to_str(t_ms *env, int argc, char *argv[])
{
	size_t	len;
	int		i;
	size_t	j;
	char	*str;

	len = ms_count_char_in_array(argv);
	str = ms_malloc(env, len, sizeof(char));
	ms_memset(str, 0, len);
	i = MS_FIRST_ARG_POS;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] != ' ')
				*str++ = argv[i][j];
			++j;
		}
		if (i < argc - 1)
			*str++ = ' ';
		++i;
	}
	*str = '\0';
	return (str - len + 1);
}

void	ppx_putstr_fd(char *str, int fd, bool option)
{
	if (str)
	{
		write(fd, str, ms_strlen(str));
		if (option == MS_PUT_NEWLINE)
			write(fd, "\n", 1);
	}
}
