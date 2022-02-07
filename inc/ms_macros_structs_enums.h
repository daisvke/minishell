/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_macros_structs_enums.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 03:16:42 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/07 03:12:50 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_MACROS_STRUCTS_ENUMS_H
# define MS_MACROS_STRUCTS_ENUMS_H

# define MS_FIRST_ARG_POS				1
# define MS_LOOP_NOT_ENDED_BY_CTRL_D	1
# define MS_EVEN						0
# define MS_ALL							0

// COLORS
# define MS_CLR_FAINT_WHITE				"\001\x1B[2m\x1B[1m\x1B[37m\002"
# define MS_CLR_BRIGHT_GREEN			"\001\x1B[1m\x1B[92m\002"
# define MS_CLR_BOLD_BLUE				"\001\x1B[1m\x1B[34m\002"

// LINKED LIST CONTAINING ENV ENTRIES
typedef struct s_env_lst
{
	char	*entry;
	void	*next;
}			t_env_lst;

// PIPEX ENV
typedef struct s_ppx
{
	int		**pipe_fds;
	size_t	pos;
	size_t	cmd_nbr;
	size_t	i;
	int		fd_in;
	int		fd_out;
	char	**cmd;
	int		options;
	size_t	heredoc_pos;
}			t_ppx;

// PROMPT
typedef struct s_prompt
{
	char	*prompt;
	size_t	home_path_len;
	char	*logname;
	char	*name;
}			t_prompt;

enum e_prompt
{
	MS_PMP_NO_HOME = -1,
	MS_PMP_AT_HOME = 0,
	MS_PMP_TILDE_LEN = 1,
	MS_PMP_DOLLAR_SPACE_LEN = 2,
	MS_PMP_FIRST_TIME = 1
};

// MINISHELL ENV
typedef struct s_ms
{
	t_ppx		ppx_env;
	t_env_lst	*envp_lst;
	t_prompt	cmd_prompt;
	char		*cmdline;
	char		**split_cmdline;
	int			options;
	int			last_pipe_exit_status;
	int			exit_status;
}				t_ms;

// DELETE REDIRECTION SECTION
typedef struct s_del
{
	size_t	line;
	size_t	pos;
	size_t	lines_to_del;
}			t_del;

// BITWISE FOR SPLIT
typedef struct s_split
{
	bool	quotes;
	bool	first_char_not_quote;
	size_t	i;
	char	sep;
	char	*start;
}			t_split;

// VARIABLES USED IN MS_EXPAND_VARIABLES
typedef struct s_expv
{
	int		status;
	bool	found_var;
	bool	double_quote;
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	start;
	size_t	len;
}			t_expv;

// RETURNS
enum e_comparison
{
	MS_SAME,
	MS_DIFFERENT
};

enum e_return
{
	MS_OK = 0,
	MS_FOUND = 0,
	MS_NOT_FOUND = 1,
	MS_SUCCESS = 0,
	MS_ERROR = 1
};

// READ FROM PROMPT
enum e_read
{
	MS_READ_LINE,
	MS_READ_EOF,
	MS_READ_NONE
};

// CLOSE AFTER DUP2
enum e_dup
{
	MS_DUP_OFF,
	MS_DUP_CLOSE_FD
};

// CLOSE PIPE FDS
enum e_close_pipe_fds
{
	MS_CPF_OFF,
	MS_CPF_AFTER_INCREM
};

// IMPLEMENTED COMMANDS
# define MS_SPACE_TO_SEP_PRINTED_ARGS	" "
# define MS_NO_EXPORT					1

enum e_cmds
{
	MS_CMD_ECHO,
	MS_CMD_CD,
	MS_CMD_PWD,
	MS_CMD_EXPORT,
	MS_CMD_UNSET,
	MS_CMD_ENV,
	MS_CMD_EXIT	
};

// ERRORS
enum e_error_print_options
{
	MS_PUT_NEWLINE,	
	MS_NONE
};

// BITWISE FOR S_MS OPTIONS
# define MS_OPT_INIT_ALL_BUT_PIPE		48

enum e_options
{
	MS_OPT_PIPE	= 16,
	MS_OPT_APPEND_OUTPUT = 8,
	MS_OPT_READ_FROM_FILE = 4,
	MS_OPT_REDIR_OUTPUT	= 2,
	MS_OPT_HEREDOC = 1
};

/*
** FROM PIPEX
*/
# define PPX_ERROR 						-1

// PROCESSES WHILE FORKING
enum e_processes
{
	PPX_PROC_CHILD,
	PPX_PROC_PARENT
};

enum e_access
{
	PPX_OFF,
	PPX_CHECK_ALL
};

#endif
