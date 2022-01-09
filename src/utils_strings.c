/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 09:21:17 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/09 10:01:15 by dtanigaw         ###   ########.fr       */
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

char	*ms_strchr(const char *s, char *to_find)
{
	size_t	i;

	if (to_find == NULL)
		return ((char *)s);
	while (*s)
	{
		i = 0;
		while (to_find[i])
		{
			if (*s == to_find[i])
				return ((char *)s);
			++i;
		}
		s++;
	}
	return (0);
}
