/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 04:07:23 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/31 12:30:55 by dtanigaw         ###   ########.fr       */
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

int	gnl_get_line(t_ppx *env, char **data, int fd)
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

int	gnl_run_and_return(t_ppx *env, char **data, char **line, int fd)
{
	int		res;
	int		index;
	char	*tmp;
	bool	is_empty;

	tmp = NULL;
	res = gnl_get_line(env, data, fd);
	if (res == PPX_ERROR)
		return (PPX_ERROR);
	index = 0;
	if (*data)
		index = gnl_get_char_index(*data, '\n', true);
	is_empty = index + 1 > gnl_get_char_index(*data, '\0', true);
	*line = gnl_concatenate(*data, NULL, index, false);
	if (!line)
		return (PPX_ERROR);
	if (*data)
	{
		tmp = gnl_concatenate(*data + index + 1, NULL, \
			gnl_get_char_index(*data, '\0', true) - index - 1, is_empty);
		if (!tmp)
			return (PPX_ERROR);
	}
	*data = ms_free(*data);
	*data = tmp;
	if (res == GNL_REACHED_EOF && is_empty)
		return (GNL_REACHED_EOF);
	else
		return (GNL_READ_LINE);
}

int	get_next_line(t_ppx *env, int fd, char **line)
{
	static char	*data;
	char		*data_cpy;
	int			res;

	if (BUFFER_SIZE <= 0 || !line)
		return (PPX_ERROR);
	data_cpy = data;
	res = gnl_run_and_return(env, &data_cpy, line, fd);
	if (res == GNL_REACHED_EOF || res == PPX_ERROR)
		data_cpy = ms_free(data_cpy);
	data = data_cpy;
	return (res);
}
