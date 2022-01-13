/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 16:29:43 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 09:58:31 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ppx_free_array_of_pointers(char **array_of_pointers, size_t arr_size)
{
	size_t	i;

	i = 0;
	if (arr_size != 0)
	{
		while (i < arr_size)
		{
			array_of_pointers[i] = ms_free(array_of_pointers[i]);
			++i;
		}
	}
	else
	{
		while (array_of_pointers[i])
		{
			array_of_pointers[i] = ms_free(array_of_pointers[i]);
			++i;
		}
	}
	array_of_pointers = ms_free(array_of_pointers);
}

size_t	ppx_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	*ppx_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	if (!dest && !src)
		return (NULL);
	while (n--)
		*d++ = *s++;
	return (dest);
}
/*
char	*ppx_join_two_str(t_ppx *env, char *str1, char *str2)
{
	char	*joined;
	size_t	joined_len;
	size_t	str1_len;
	size_t	str2_len;

	str1_len = ppx_strlen(str1);
	str2_len = ppx_strlen(str2);
	joined_len = str1_len + str2_len;
	joined = ppx_malloc(env, joined_len + 1, sizeof(*joined));
	ppx_memcpy(joined, str1, str1_len);
	ppx_memcpy(&joined[str1_len], str2, str2_len);
	joined[joined_len] = '\0';
	return (joined);
}
*/
char	*ppx_join_three_str(t_ppx *env, char *str1, char *str2, char *str3)
{
	char	*joined;
	size_t	joined_len;
	size_t	str1_len;
	size_t	str2_len;
	size_t	str3_len;

	if (!str1 || !str2 || !str3)
		ppx_exit_with_error_message(env, 8);
	str1_len = ppx_strlen(str1);
	str2_len = ppx_strlen(str2);
	str3_len = ppx_strlen(str3);
	joined_len = str1_len + str2_len + str3_len;
	joined = ppx_malloc(env, joined_len + 1, sizeof(*joined));
	ppx_memcpy(joined, str1, str1_len);
	ppx_memcpy(&joined[str1_len], str2, str2_len);
	ppx_memcpy(&joined[str1_len + str2_len], str3, str3_len);
	joined[joined_len] = '\0';
	return (joined);
}

int	ppx_strncmp(const char *s1, const char *s2, size_t n)
{
	if (s1 && s2)
	{
		while ((*s1 || *s2) && n--)
		{
			if (*s1 != *s2)
				return ((unsigned char)(*s1) - (unsigned char)(*s2));
			s1++;
			s2++;
		}
		return (MS_SAME);
	}
	return (MS_DIFFERENT);
}
