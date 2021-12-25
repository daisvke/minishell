/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 07:13:24 by dtanigaw         ###   ########.fr       */
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

// READ FROM PROMPT
# define MS_LINE_READ		0
# define MS_EMPTY_CMD_LINE	1 

// CHECK IF THE CMD IS IMPLEMENTED
# define MS_CMD_CD			1
# define MS_CMD_EXPORT		2
# define MS_CMD_UNSET		3
# define MS_CMD_EXIT		4
# define MS_FIRST_ARG_POS	1

# define MS_ERROR			1

// Main struct
typedef struct s_ms
{
	char	**cmd_line;
	bool	pipe;
}			t_ms;


/*
** utils_strings
*/
int	ms_strncmp(const char *s1, const char *s2, size_t n);

char	**ms_split_and_activate_options(t_ms *env, char const *s, char c);

#endif
