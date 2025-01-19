/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls_string_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 06:06:40 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/24 12:36:00 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ms_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ms_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return (MS_DIFFERENT);
		s1++;
		s2++;
	}
	return (MS_SAME);
}

int	ms_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!s1 || !s2 || !*s1 || !*s2)
		return (MS_DIFFERENT);
	while ((*s1 || *s2) && n--)
	{
		if (*s1 != *s2)
			return (MS_DIFFERENT);
		s1++;
		s2++;
	}
	return (MS_SAME);
}

void	*ms_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	str = s;
	i = 0;
	while (i < n)
		str[i++] = (unsigned char)c;
	return (s);
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
