NAME				=	minishell

CC					=	clang $(INC) $(WFLAGS) -g3 $(MEM)
WFLAGS				=	-Wall -Wextra #-Werror
READ				=	-lreadline	
INC					=	-I inc/
MEM					=	-fsanitize=address -fsanitize=undefined

SRC_DIR				=	src/
SRC_FILES			=	builtin_cmds_1.c \
						builtin_cmds_1_cd.c \
						builtin_cmds_2_export.c \
						builtin_cmds_2_unset.c \
						builtin_cmds_utils.c \
						child_proc.c \
						exit_minishell.c \
						exit_pipex.c \
						exit_utils.c \
						expand_variables.c \
						expand_variables_utils_1.c \
						expand_variables_utils_2.c \
						file_handler.c \
						get_next_line.c \
						heredoc.c \
						linked_list.c \
						linked_list_utils.c \
						minishell.c \
						minishell_utils_1.c \
						minishell_utils_2.c \
						ms_init_env.c \
						path.c \
						pipex.c \
						pipex_utils.c \
						ppx_init_env.c \
						prompt.c \
						redirections.c \
						redirections_utils.c \
						signals.c \
						split.c \
						split_utils.c \
						split_and_activate_options.c \
						system_calls_stdlib.c \
						system_calls_string_1.c \
						system_calls_string_2.c \
						system_calls_pipex.c \
						system_calls_unistd.c \
						utils_nbr.c \
						utils_strings_1.c \
						utils_strings_2.c
SRC					=	$(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR				=	obj/
OBJ_FILES			=	$(SRC_FILES:.c=.o)
OBJ					=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

RM					= rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION COMPLETED]\033[0m\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p obj/
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
