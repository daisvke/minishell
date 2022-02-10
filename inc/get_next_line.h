/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 04:07:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/10 03:26:32 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

/**************************************
			M A C R O S
**************************************/

# define GNL_NOT_FOUND		-1
# define GNL_OFF			-1

/**************************************
	B U F F E R  F O R  R E A D ( )
**************************************/

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 		32
# endif

/**************************************
	F O R  G E T _ L I N E ()
**************************************/

typedef struct s_gnl
{
	int		res;
	int		i;
	char	*tmp;
	bool	is_empty;
}			t_gnl;

/**************************************
	R E T U R N  V A L U E S
**************************************/

enum e_gnl_return
{
	GNL_REACHED_EOF,
	GNL_READ_LINE
};

/**************************************
 F U N C T I O N  P R O T O T Y P E S
**************************************/

int	get_next_line(int fd, char **line);

#endif
