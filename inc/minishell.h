/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/06 22:49:57 by dtanigaw         ###   ########.fr       */
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

# include "ms_macros_structs_enums.h"

# include "pipex.h"

/*
** system calls
*/
void	*ms_free(void *data);

/*
** utils: strings
*/
int	ms_strncmp(const char *s1, const char *s2, size_t n);

char	*ms_strdup(char *src, size_t size); //used
char	**ms_split_and_activate_options(t_ms *env, char const *s, char sep);

#endif
