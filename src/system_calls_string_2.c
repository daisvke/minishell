/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls_string_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 09:37:06 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 01:39:03 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ms_strdup_with_exit(t_ms *env, char *src, size_t size)
{
	size_t	i;
	char	*dest;

	dest = malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		ms_exit_with_error_message(env, 11);
	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
