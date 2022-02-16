/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 06:18:38 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/16 23:31:51 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/**************************************
			H E A D E R S
**************************************/

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "ms_macros_structs_enums.h"
# include "pipex.h"

/**************************************
	 G L O B A L  V A R I A B L E
**************************************/

extern int	g_exit_status;

/**************************************
 F U N C T I O N  P R O T O T Y P E S
**************************************/

//			 SYSTEM CALLS

void		*ms_free(void *data);
void		*ms_malloc(t_ms *env, size_t num, size_t size);
void		*ms_memset(void *s, int c, size_t n);
void		ms_close(t_ms *env, int fd);
void		ms_dup2(t_ms *env, int fd1, int fd2);
void		ms_pipe(t_ms *env, int *fds);
int			ms_strcmp(const char *s1, const char *s2);
char		*ms_strdup_with_exit(t_ms *env, char *src, size_t size);
int			ms_strncmp(const char *s1, const char *s2, size_t n);
char		*ms_strdup(char *src, size_t size); //used
size_t		ms_strlen(const char *s);

//			INIT

void		ms_assign_key_value_to_cmd_prompt(\
	char **var, char *entry, size_t key_len);
char		*ms_convert_array_of_str_to_str(t_ms *env, int argc, char *argv[]);
char		*ms_get_pwd(t_ms *env, t_env_lst *path_node, size_t *key_len);
char		*ms_handle_case_null(size_t *key_len, bool *malloced);
void		ms_handle_signals(void);
void		ms_init_cmd_prompt(t_ms *env);
void		ms_init_env(int argc, char *envp[], t_ms *env);
void		ms_set_var_according_to_envp_entry(\
	t_ms *env, char **var, char *key);
void		ms_set_variables_for_cmd_prompt(t_ms *env, t_prompt *cmd_prompt);

//			ENVP

char		**ms_convert_envp_lst_to_array_of_pointers(\
	t_ms *env, t_env_lst *envp_lst, size_t lst_size);
void		ms_convert_envp_into_linked_list(char *envp[], t_ms *env);
t_env_lst	*ms_lst_get_node_with_the_same_key(t_env_lst *envp_lst, char *key);
void		ms_lst_add_back(t_env_lst *head, t_env_lst *new);
void		ms_lst_assign_entry_to_node(t_env_lst *node, char *entry);
void		ms_lst_clear_list(t_env_lst *node);
t_env_lst	*ms_lst_create_new_node(t_ms *env, char *data);
void		ms_lst_del_node(t_env_lst *node);
t_env_lst	*ms_lst_get_last_node(t_env_lst *node);
int			ms_lst_lstsize(t_env_lst *head);

//			PARSING

int			ms_check_arguments(char *envp[], int argc);
int			ms_check_if_quote_nbr_is_even(char *cmdline);
int			ms_check_pipes_and_redirections(t_ms *env, char *cmdline);
int			ms_compare_with_envp_key(\
	const char *envp_entry, const char *str, bool equal_in_str);
int			ms_parse_cmdline(t_ms *env, char **cmdline);

//			COMMAND PROMPT

void		ms_generate_new_path_for_prompt(\
	t_ms *env, char *current_path, int len, bool first_time);
char		*ms_get_home_value_from_envp_lst(t_ms *env);
void		ms_print_only_slash_and_dollar_when_before_home(t_ms *env);
void		ms_print_only_tilde_and_dollar_when_at_home(t_ms *env);
void		ms_update_prompt(\
	t_ms *env, t_env_lst *envp_lst, t_prompt *cmd_prompt, bool first_time);
void		ms_set_first_part_of_cmd_prompt(\
	t_ms *env, t_prompt *cmd_prompt, bool first_time);

//			EXPAND VARIABLES

bool		ms_begins_with_dollar_or_dollar_is_not_preceded_by_quote(\
	char *cmdline, t_expv *vars);
void		ms_copy_value_to_the_expansion_location(\
	char *value, char *location, t_expv *vars);
void		ms_expand_exit_status(\
	t_ms *env, char *new_cmdline, t_expv *vars);
char		*ms_expand_variables(t_ms *env, char *cmdline, t_expv *vars);
bool		ms_found_exit_status_symbols(\
	char *cmdline, t_expv *vars);
char		*ms_get_envp_value_from_key(t_ms *env, char *key);
void		ms_get_exit_status_length(t_expv *vars);
char		*ms_print_last_exit_status(t_ms *env);

//			REDIRECTIONS

void		ms_apply_append_mode(t_ppx *env, char *file);
void		ms_apply_heredoc(\
	t_ppx *env, char *file, size_t hd_count, size_t hd_total);
char		*ppx_check_outfile(\
	t_ppx *env, char *file, size_t i, size_t *lines_to_del);

//			COMMANDS

bool		ms_check_if_the_cmd_is_implemented(\
	char **cmdline, size_t *cmd_code, bool process);
bool		ms_check_if_there_is_not_too_much_args(char **cmd_and_args);
void		ms_execute_cmd_cd(t_ms *ms_env, t_ppx *ppx_env, char *arg_path);
void		ms_execute_cmd_echo(char *cmd[]);
void		ms_execute_cmd_env(t_env_lst *envp_head);
void		ms_execute_cmd_export(t_ms *env, char *cmdline[]);
void		ms_execute_cmd_pwd(t_env_lst *envp_lst);
void		ms_execute_cmd_unset(t_ms *env, char *cmdline[]);
void		ms_execute_cmdline_with_pipex(t_ms *env, char **cmdline);
void		ms_print_not_valid_identifier_err_message(char *cmd);
int			ms_run_readline(t_ms *env, char *read_line);
void		ms_update_prompt_when_home_is_unset(t_ms *env, bool first_time);

//			UTILS: NUMBERS

int			ms_get_absolute_value(long long int nbr);
char		*ms_itoa(t_ms *env, int n);
size_t		ms_nbrlen(long long int n);

//			UTILS: STRINGS

char		ms_check_if_char_is_a_redir_symbol(int c);
char		*ms_color_string(t_ms *env, char *str, char *color);
size_t		ms_handle_quotes(char *str, char quote);
bool		ms_isalnum(int c);
bool		ms_isalpha(int c);
bool		ms_isdigit(int c);
void		ms_print_if_not_backslash(char *arg);
void		ms_reset_color_settings(void);
char		*ms_search_redir_symbol(char *str);
char		**ms_split_and_activate_options(t_ms *env, char const *s, char sep);

//			EXIT

void		ms_close_pipe_fds(t_ms *ms_env, t_ppx *ppx_env, bool after_increm);
void		ms_exit_with_error_message(t_ms *env, int err_code);
void		ms_free_all_allocated_variables(t_ms *env);
void		ms_free_prompt_struct(t_ms *env);
void		ms_print_error_message(int err_code);
void		ms_quit_with_ctrl_d(t_ms *env);

#endif
