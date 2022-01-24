/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls_string_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 09:37:06 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/24 01:06:49 by dtanigaw         ###   ########.fr       */
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

char	*ppx_strdup_with_exit(t_ppx *env, char *src, size_t size)
{
	size_t	i;
	char	*dest;

	dest = malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		ppx_exit_with_error_message(env, 7);
	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
