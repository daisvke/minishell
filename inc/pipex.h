/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/06 21:12:46 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# include "get_next_line.h"

/*
** init
*/
void	ppx_init_ppx(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_and_file_nbr);

//int		ppx_pipex(char *argv[], char *envp[], t_ppx *env, t_ms *ms_env);

/*
** system calls
*/
void	ppx_close(t_ppx *env, int fd);
void	ppx_dup2(t_ppx *env, int fd1, int fd2);
pid_t	ppx_fork(t_ppx *env);
void	*ppx_malloc(t_ppx *env, size_t num, size_t size);
void	ppx_pipe(t_ppx *env, int *fds);

/*
** signals
*/
void	ms_handle_signals(t_ms *ms_env);

/*
** utils_fd
*/
void	ppx_get_fd(t_ppx *env, char *argv[]);
int		ppx_open_file(t_ppx *env, char *file_name, int flags, int mod);
void	ppx_putstr_fd(char *s, int fd, bool option);

/*
** path
*/
bool	ppx_check_access(char *path);
char	*ppx_get_the_right_cmd_path(t_ms *ms_env, t_ppx *ppx_env, \
	char *key, char *cmd);

/*
** exit
*/
void	ppx_exit_when_cmd_not_found(t_ppx *env, char *cmd);
void	ppx_exit_with_error_message(t_ppx *env, int err_code);
void	ppx_free_pipe_fds(t_ppx *env);

/*
** utils_str
*/
void	ppx_free_array_of_pointers(char **array_of_pointers, size_t arr_size);
char	*ppx_join_three_str(t_ppx *env, char *str1, char *str2, char *str3);
size_t	ppx_strlen(const char *s);
int		ppx_strncmp(const char *s1, const char *s2, size_t n);
void	*ppx_memcpy(void *dest, const void *src, size_t n);

/*
** split
*/
void	ppx_free_split(char **array_of_pointers);
char	**ppx_split(char const *s, char c);

/*
** redirections
*/
void	ppx_handle_redirections(t_ppx *env);

/*
** heredoc
*/
int		ppx_get_open_flags(t_ppx *env);
void	ppx_request_heredoc_input(t_ppx *env, char *limiter);

#endif
