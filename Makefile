NAME				=	minishell

CC					=	clang $(INC) $(WFLAGS) -g3 
WFLAGS				=	-Wall -Wextra #-Werror
READ				=	-lreadline	
INC					=	-I inc/

SRC_DIR				=	src/
SRC_FILES			=	exit.c \
						init_ms_env.c \
						main.c \
						path.c \
						ppx_main.c \
						split.c \
						split_and_activate_options.c \
						get_next_line.c \
						heredoc.c \
						pipex.c \
						system_calls.c \
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
	@$(CC) $(OBJ) $(READ) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p obj/
	@$(CC) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
