/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/27 06:09:59 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# include "pipex.h"

# define MS_SAME			0
# define MS_OK				0
# define MS_ERROR			1

# define MS_LOOP_NOT_ENDED_BY_CTRL_D	1

// READ FROM PROMPT
# define MS_READ_LINE		0
# define MS_READ_EOF		1

// CHECK IF THE CMD IS IMPLEMENTED
# define MS_CMD_CD			1
# define MS_CMD_PWD			2
# define MS_CMD_EXPORT		3
# define MS_CMD_UNSET		4
# define MS_CMD_EXIT		5
# define MS_FIRST_ARG_POS	1

// Main struct
typedef struct s_ms
{
	char	*current_directory;
	char	**cmd_line;
	bool	pipe;
}			t_ms;


/*
** utils_strings
*/
int	ms_strncmp(const char *s1, const char *s2, size_t n);

char	*ms_strdup(char *src, size_t size); //used
char	**ms_split_and_activate_options(t_ms *env, char const *s, char c);

#endif
