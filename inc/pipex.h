/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 01:51:01 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/**************************************
			H E A D E R S
**************************************/

# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# include "get_next_line.h"

/**************************************
	M A C R O S  &  S T R U C T S
**************************************/

# define PPX_ERROR 	-1

//	PROCESSES

enum e_processes
{
	PPX_PROC_CHILD,
	PPX_PROC_PARENT
};

//	CHECK ACCESS TO FILES

enum e_access
{
	PPX_OFF,
	PPX_CHECK_ALL
};

/**************************************
 F U N C T I O N  P R O T O T Y P E S
**************************************/

//		INIT

void	ppx_init_ppx(t_ms *ms_env, t_ppx *ppx_env, size_t cmd_and_file_nbr);

//		SYSTEM CALLS

void	ms_close(t_ms *env, int fd);
pid_t	ms_fork(t_ms *env);
void	ms_pipe(t_ms *env, int *fds);

//		UTILS: FD

char	*ppx_check_outfile(\
	t_ms *env, char *file, size_t i, size_t *lines_to_del);
char	*ppx_generate_filename(t_ms *env, bool increment);
void	ppx_get_fd(t_ppx *env, char *argv[]);
int		ppx_get_open_flags(t_ppx *env);
int		ppx_open_file(t_ms *env, char *file_name, int flags, int mod);
void	ppx_putstr_fd(char *s, int fd, bool option);

//		EXECUTE COMMAND

void	ppx_execute_implemented_cmd_in_parent(\
	t_ms *ms_env, t_ppx *ppx_env, size_t cmd_code, char *cmd[]);
void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env);

//		PATH

bool	ppx_check_access(char *path, bool check_all);
char	*ppx_get_the_right_cmd_path(t_ms *ms_env, char *key, char *cmd);

//		PIPE

int		ppx_create_array_of_commands(\
	t_ms *ms_env, t_ppx *ppx_env, char *cmdline[]);
bool	ms_pipe_is_off_and_cmd_is_implemented(t_ppx *env, size_t *cmd_code);
void	ppx_wait_for_all_children(\
	t_ms *ms_env, t_ppx *ppx_env, pid_t pid, size_t wait_count);
void	ppx_wait_for_proc_with_heredoc(pid_t pid, size_t *wait_count);

//		EXIT

void	ppx_close_pipe_fds(t_ppx *env);
void	ppx_exit_when_cmd_not_found(t_ms *env, char *cmd);
void	ppx_free_all_allocated_variables(t_ppx *env);
void	ppx_free_pipe_fds(t_ppx *env);

//		UTILS_STR

void	ppx_free_array_of_pointers(char ***array_of_pointers, size_t arr_size);
char	*ppx_join_three_str(t_ms *env, char *str1, char *str2, char *str3);
void	*ppx_memcpy(void *dest, const void *src, size_t n);

//		SPLIT

bool	ppx_check_if_str_contains_quotes(char *str);
int		ppx_check_quotes(char *str, char c, bool *quotes);
char	**ppx_split(char const *s, char c);
int		ppx_wordcount(char *str, int sep);

//		REDIRECTIONS

void	ppx_handle_redirections(t_ms *ms_env, t_ppx *ppx_env);
bool	ppx_is_a_line_to_del_not_starting_with_a_redir_symbol(\
	t_del del, size_t i, t_ppx *env);
bool	ppx_is_not_a_line_to_del(t_del del, size_t i);
void	ms_read_from_file(t_ms *ms_env, t_ppx *ppx_env, char *file);
void	ms_redirect_output(t_ms *ms_env, t_ppx *ppx_env, char *file);

//		HEREDOC

size_t	ppx_count_heredoc(char *cmd[]);
void	ppx_detect_heredocs(t_ppx *env, char *cmd[]);
int		ppx_get_open_flags(t_ppx *env);

#endif
