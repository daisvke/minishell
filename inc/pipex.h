/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/09 21:11:25 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# include "get_next_line.h"

/*
** init
*/
void	ppx_init_ppx(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_and_file_nbr);

/*
** system calls
*/
void	ppx_close(t_ppx *env, int fd);
void	ppx_dup2(t_ppx *env, int fd1, int fd2, bool close_fd);
pid_t	ppx_fork(t_ms *env);
void	*ppx_malloc(t_ppx *env, size_t num, size_t size);
void	ppx_pipe(t_ms *env, int *fds);

/*
** utils_fd
*/
char	*ppx_check_outfile(\
	t_ppx *env, char *file, size_t i, size_t *lines_to_del);
char	*ppx_generate_filename(t_ppx *env, bool increment);
void	ppx_get_fd(t_ppx *env, char *argv[]);
int		ppx_get_open_flags(t_ppx *env);
int		ppx_open_file(t_ppx *env, char *file_name, int flags, int mod);
void	ppx_putstr_fd(char *s, int fd, bool option);

/*
** execute command
*/
void	ppx_execute_implemented_cmd_in_parent(\
	t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[]);
void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env);

/*
** path
*/
bool	ppx_check_access(char *path, bool check_all);
char	*ppx_get_the_right_cmd_path(t_ms *ms_env, t_ppx *ppx_env, \
	char *key, char *cmd);

/*
** pipe
*/
int		ppx_create_array_of_commands(\
	t_ms *ms_env, t_ppx *ppx_env, char *cmdline[]);
bool	ppx_pipe_is_off_and_cmd_is_implemented(t_ppx *env, size_t *cmd_code);
void	ppx_save_data_from_child(t_ms *ms_env, t_ppx *ppx_env);

/*
** exit
*/
void	ppx_close_pipe_fds(t_ppx *env);
void	ppx_exit_when_cmd_not_found(t_ms *env, char *cmd, char *path_to_cmd);
void	ppx_exit_with_error_message(t_ppx *env, int err_code);
void	ppx_free_all_allocated_variables(t_ppx *env);
void	ppx_free_pipe_fds(t_ppx *env);

/*
** utils_str
*/
void	ppx_free_array_of_pointers(char ***array_of_pointers, size_t arr_size);
char	*ppx_itoa(t_ppx *env, int n);
char	*ppx_join_three_str(t_ppx *env, char *str1, char *str2, char *str3);
void	*ppx_memcpy(void *dest, const void *src, size_t n);
char	*ppx_strdup_with_exit(t_ppx *env, char *src, size_t size);

/*
** split
*/
bool	ppx_check_if_str_contains_quotes(char *str);
int		ppx_check_quotes(char *str, char c, bool *quotes);
char	**ppx_split(char const *s, char c);
int		ppx_wordcount(char *str, int sep);

/*
** redirections
*/
void	ppx_handle_redirections(t_ppx *env);
bool	ppx_is_a_line_to_del_not_starting_with_a_redir_symbol(\
	t_del del, size_t i, t_ppx *env);
bool	ppx_is_not_a_line_to_del(t_del del, size_t i);

/*
** heredoc
*/
size_t	ppx_count_heredoc(char *cmd[]);
void	ppx_detect_heredocs(t_ppx *env, char *cmd[]);
int		ppx_get_open_flags(t_ppx *env);

#endif
