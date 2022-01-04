/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/03 10:34:46 by dtanigaw         ###   ########.fr       */
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
# include <termios.h>

# include "pipex.h"

# define MS_SAME			0
# define MS_OK				0
# define MS_SUCCESS			0
# define MS_ERROR			1

# define MS_LOOP_NOT_ENDED_BY_CTRL_D	1

// READ FROM PROMPT
# define MS_READ_LINE		0
# define MS_READ_EOF		1
# define MS_READ_NONE		2

// CHECK IF THE CMD IS IMPLEMENTED
# define MS_CMD_ECHO		1
# define MS_CMD_CD			2
# define MS_CMD_PWD			3
# define MS_CMD_EXPORT		4
# define MS_CMD_UNSET		5
# define MS_CMD_ENV			6
# define MS_CMD_EXIT		7
# define MS_FIRST_ARG_POS	1

# define MS_NO_EXPORT		1

// BITWISE FOR S_MS OPTIONS
# define MS_OPT_PIPE			4
# define MS_OPT_REDIR_OUTPUT	2
# define MS_OPT_HEREDOC			1

// Linked list containing env entries
typedef struct s_env_lst
{
	char	*entry;
	void	*next;
}			t_env_lst;

// Main struct
typedef struct s_ms
{
	t_env_lst	*envp_lst;
	char	**envp;
	char	*cmd_line;
	char	**split_cmd_line;
	int		options;
	int		last_pipe_exit_status;
}			t_ms;

/*
** Strings utils
*/
int	ms_strncmp(const char *s1, const char *s2, size_t n);

char	*ms_strdup(char *src, size_t size); //used
char	**ms_split_and_activate_options(t_ms *env, char const *s, char sep);

/*
** Linked lists
*/
t_env_lst	*ms_lst_create_new_node(char *data);

#endif
