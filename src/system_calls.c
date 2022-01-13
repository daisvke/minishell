/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 22:37:23 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/09/27 05:13:54 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	*ppx_malloc(t_ppx *env, size_t num, size_t size)
{
	void	*ptr;

	ptr = (void *)malloc(num * size);
	if (!ptr)
		ppx_exit_with_error_message(env, 1);
	return (ptr);
}

void	*ms_malloc(t_ms *env, size_t num, size_t size)
{
	void	*ptr;

	ptr = (void *)malloc(num * size);
	if (!ptr)
		ms_exit_with_error_message(env, 1);
	return (ptr);
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

void	*ms_free(void *data)
{
	if (data)
		free(data);
	return (NULL);
}
