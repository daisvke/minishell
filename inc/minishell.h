/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/21 19:57:16 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "pipex.h"

# define MS_SAME			0
# define MS_NO_ARGS			0

# define MS_LINE_READ		0
# define MS_EMPTY_CMD_LINE	1 

# define MS_ERROR			1

// Main struct
typedef struct t_ms
{
}


/*
** utils_strings
*/
int	ms_strncmp(const char *s1, const char *s2, size_t n);

#endif
