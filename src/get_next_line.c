/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 04:07:23 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/10 03:23:31 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	gnl_get_char_index(char *str, char c, bool increment)
{
	size_t	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (i);
		++i;
	}
	if (increment)
		return (i);
	else
		return (GNL_NOT_FOUND);
}

char	*gnl_concatenate(char *s1, char *s2, int len, bool is_empty)
{
	char	*str;
	size_t	i;
	size_t	size;

	size = 0;
	if (s1 && is_empty == false)
		size += gnl_get_char_index(s1, '\0', true);
	if (s2)
		size += gnl_get_char_index(s2, '\0', true);
	if (len >= 0 && (int)size > len)
		size = len;
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (is_empty == false && s1 && *s1 && i < size)
		str[i++] = *s1++;
	while (s2 && *s2 && i < size)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}

int	gnl_get_line(char **data, int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	int			res;
	char		*tmp;

	res = 0;
	while (gnl_get_char_index(*data, '\n', false) == GNL_NOT_FOUND)
	{
		res = read(fd, buffer, BUFFER_SIZE);
		if (res < 0)
			return (PPX_ERROR);
		if (res == GNL_REACHED_EOF)
			break ;
		buffer[res] = '\0';
		tmp = gnl_concatenate(*data, buffer, GNL_OFF, false);
		if (!tmp)
			return (PPX_ERROR);
		*data = ms_free(*data);
		*data = tmp;
	}
	return (res);
}

int	gnl_run_and_return(t_gnl env, char **data, char **line, int fd)
{
	env.res = gnl_get_line(data, fd);
	if (env.res == PPX_ERROR)
		return (PPX_ERROR);
	if (*data)
		env.i = gnl_get_char_index(*data, '\n', true);
	env.is_empty = env.i + 1 > gnl_get_char_index(*data, '\0', true);
	*line = gnl_concatenate(*data, NULL, env.i, false);
	if (!line)
		return (PPX_ERROR);
	if (*data)
	{
		env.tmp = gnl_concatenate(*data + env.i + 1, NULL, \
			gnl_get_char_index(*data, '\0', true) - env.i - 1, env.is_empty);
		if (!env.tmp)
			return (PPX_ERROR);
	}
	*data = ms_free(*data);
	*data = env.tmp;
	if (env.res == GNL_REACHED_EOF && env.is_empty)
		return (GNL_REACHED_EOF);
	else
		return (GNL_READ_LINE);
}

int	get_next_line(int fd, char **line)
{
	static char	*data;
	char		*data_cpy;
	t_gnl		env;
	int			res;

	if (BUFFER_SIZE <= 0 || !line)
		return (PPX_ERROR);
	data_cpy = data;
	ms_memset(&env, 0, sizeof(t_gnl));
	res = gnl_run_and_return(env, &data_cpy, line, fd);
	if (res == GNL_REACHED_EOF || res == PPX_ERROR)
		data_cpy = ms_free(data_cpy);
	data = data_cpy;
	return (res);
}
