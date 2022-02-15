# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/15 03:32:39 by dtanigaw          #+#    #+#              #
#    Updated: 2022/02/15 04:50:39 by dtanigaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#	 E X E C U T A B L E  N A M E	 #

NAME				=	minishell


#			C C  F L A G S			  #

CC					=	clang $(INC) $(WFLAGS)
WFLAGS				=	-Wall -Wextra -Werror
READ				=	-lreadline	
INC					=	-I inc/
MEM					=	-fsanitize=address -fsanitize=undefined


#		S O U R C E  F I L E S		  #

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
						ms_init_env_utils.c \
						path.c \
						pipex.c \
						pipex_utils.c \
						ppx_init_env.c \
						prompt.c \
						prompt_utils.c \
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


#			O B J .  F I L E S		  #

OBJ_DIR				=	obj/
OBJ_FILES			=	$(SRC_FILES:.c=.o)
OBJ					=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))


#			B U I L D  R U L E S	  #

all: $(NAME)

# With -g3:
g: fclean $(OBJ)
	$(CC) -g3 $(OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION WITH -G3 COMPLETED]\033[0m\n"

# With -g3 & -fsanitize:
gf: fclean $(OBJ)
	$(CC) -g3 $(MEM) $(OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION WITH -G3 & -FSANITIZE COMPLETED]\033[0m\n"

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION COMPLETED]\033[0m\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p obj/
	$(CC) -c $< -o $@


# C L E A N  &  O T H E R  R U L E S  #

RM					= 	rm -rf

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
