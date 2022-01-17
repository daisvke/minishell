/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/17 02:05:32 by dtanigaw         ###   ########.fr       */
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
void		*ms_free(void *data);
void		*ms_malloc(t_ms *env, size_t num, size_t size);
void		*ms_memset(void *s, int c, size_t n);

/*
** system calls unistd
*/
void		ms_close(t_ms *env, int fd);
void		ms_dup2(t_ms *env, int fd1, int fd2);
void		ms_pipe(t_ms *env, int *fds);

/*
** signals
*/
void		ms_handle_signals(void);

/*
** init
*/
void		ms_init_env(char *envp[], t_ms *env);

/*
** envp
*/
char		**ms_convert_envp_lst_to_array_of_pointers(\
	t_ms *env, t_env_lst *envp_lst, size_t lst_size);
t_env_lst	*ms_lst_get_node_with_the_same_key(t_env_lst *envp_lst, char *key);
void		ms_lst_add_back(t_env_lst *head, t_env_lst *new);
void		ms_lst_assign_entry_to_node(\
	t_ms *env, t_env_lst *node, char *entry);
void		ms_lst_clear_list(t_env_lst **lst);
t_env_lst	*ms_lst_create_new_node(t_ms *env, char *data);
void		ms_lst_del_node(t_env_lst *node);
t_env_lst	*ms_lst_get_last_node(t_env_lst *node);
int			ms_lst_lstsize(t_env_lst *head);

/*
** parsing
*/
int			ms_check_arguments(int argc, char *argv[], char *envp[], t_ms *env);
int			ms_check_pipes_and_redirections(t_ms *env, char *cmd_line);
int			ms_compare_with_envp_key(\
	const char *envp_entry, const char *str, bool equal_in_str);
char		*ms_expand_variables(t_ms *env, char *cmd_line);

/*
** redirections
*/
void		ms_apply_append_mode(t_ppx *env, char *file);
void		ms_apply_heredoc(t_ppx *env, char *file);
char		*ppx_check_outfile(\
	t_ppx *env, char *file, size_t i, size_t *lines_to_del);

/*
** commands
*/
bool		ms_check_if_the_cmd_is_implemented(\
	char **cmd_line, size_t *cmd_code, bool process);
bool		ms_check_if_there_is_not_too_much_args(char **cmd_and_args);
void		ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *path);
void		ms_execute_cmd_echo(char *cmd[]);
void		ms_execute_cmd_env(t_env_lst *envp_head);
void		ms_execute_cmd_export(t_ms *env, char *cmd_line[]);
void		ms_execute_cmd_pwd(t_env_lst *envp_lst);
void		ms_execute_cmd_unset(t_ms *env, char *cmd_line[]);

/*
** utils: strings
*/
char		ms_check_if_char_is_a_redir_symbol(int c);
size_t		ms_handle_quotes(void *env, char *str, char quote);
bool		ms_isalnum(int c);
bool		ms_isalpha(int c);
bool		ms_isdigit(int c);
char		*ms_itoa(t_ms *env, int n);
char		*ms_search_redir_symbol(char *str);
int			ms_strcmp(const char *s1, const char *s2);
int			ms_strncmp(const char *s1, const char *s2, size_t n);
char		*ms_strdup(char *src, size_t size); //used
size_t		ms_strlen(const char *s);
char		**ms_split_and_activate_options(t_ms *env, char const *s, char sep);

/*
** exit
*/
void		ms_exit_with_error_message(t_ms *env, int err_code);
void		ms_print_error_message(int err_code);

#endif
