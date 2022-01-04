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

# include "ms_macros_and_structs.h"
# include "pipex.h"

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
