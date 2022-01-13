/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 04:07:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/09/23 04:25:59 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 	32
# endif

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

# define GNL_NOT_FOUND	0

# define GNL_OFF		-1

/*
** return values
*/
enum e_gnl_return
{
	GNL_REACHED_EOF, 
	GNL_READ_LINE
};

int		get_next_line(int fd, char **line);

#endif
