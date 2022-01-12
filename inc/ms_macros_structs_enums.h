#ifndef MS_MACROS_AND_STRUCTS_H
# define MS_MACROS_AND_STRUCTS_H

// RETURNS
enum e_comparison
{
	MS_SAME,
	MS_DIFFERENT
};

enum e_return
{
	MS_SUCCESS,
	MS_ERROR
};

# define MS_OK				0

# define MS_NO_EXPORT		1

# define MS_LOOP_NOT_ENDED_BY_CTRL_D	1
# define MS_FIRST_ARG_POS		1

// Linked list containing env entries
typedef struct s_env_lst
{
	char	*entry;
	void	*next;
}			t_env_lst;

typedef struct s_ppx
{
	int		**pipe_fds;
	int		pos;
	int		cmd_nbr;
	int		i;
	int		fd_in;
	char	**cmd;
	int		options;
}			t_ppx;

// Main struct
typedef struct s_ms
{
	t_ppx	ppx_env;
	t_env_lst	*envp_lst;
	char	**envp;
	char	*cmd_line;
	char	**split_cmd_line;
	int		options;
	int		last_pipe_exit_status;
}			t_ms;

// READ FROM PROMPT
enum e_read
{
	MS_READ_LINE,
	MS_READ_EOF,
	MS_READ_NONE
};

// CHECK IF THE CMD IS IMPLEMENTED
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

// BITWISE FOR S_MS OPTIONS
enum e_options
{
	MS_OPT_PIPE	= 16,
	MS_OPT_APPEND_OUTPUT = 8,
	MS_OPT_READ_FROM_FILE = 4,
	MS_OPT_REDIR_OUTPUT	= 2,
	MS_OPT_HEREDOC = 1
};

#endif
