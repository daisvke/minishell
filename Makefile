NAME				=	minishell

CC					=	clang $(INC) $(WFLAGS) -g3 
WFLAGS				=	-Wall -Wextra #-Werror
READ				=	-lreadline	
INC					=	-I inc/
MEM					=	fsanitize=address fsanitize=undefined

SRC_DIR				=	src/
SRC_FILES			=	builtin_cmds_1.c \
						builtin_cmds_2.c \
						child_proc.c \
						exit_minishell.c \
						exit_pipex.c \
						expand_variables.c \
						linked_list.c \
						linked_list_utils.c \
						main.c \
						path.c \
						split.c \
						split_and_activate_options.c \
						get_next_line.c \
						heredoc.c \
						ms_init_env.c \
						pipex.c \
						ppx_init_env.c \
						signals.c \
						system_calls.c \
						system_calls_pipex.c \
						utils_fd.c \
						utils_str.c \
						utils_strings.c
SRC					=	$(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR				=	obj/
OBJ_FILES			=	$(SRC_FILES:.c=.o)
OBJ					=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

RM					= rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION FINISHED]\033[0m\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p obj/
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
