# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/15 03:32:39 by dtanigaw          #+#    #+#              #
#    Updated: 2022/02/17 00:53:55 by dtanigaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
# make		=> make with WFLAGS
# make g	=> make with -g3 without -Werror
# make gf	=> make with -g3 and fsanitize, without -Werror
#


#	 E X E C U T A B L E  N A M E	 #


NAME				=	minishell


#			C C  F L A G S			  #


CC					=	clang $(INC) $(WFLAGS)

WFLAGS				=	-Wall -Wextra -Werror
READ				=	-lreadline	
INC					=	-I inc/
MEM					=	-fsanitize=address -fsanitize=undefined


#######################################
#				F I L E S			  #
#######################################


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
						system_calls_unistd.c \
						utils_nbr.c \
						utils_strings_1.c \
						utils_strings_2.c

SRC					=	$(addprefix $(SRC_DIR), $(SRC_FILES))


#			O B J .  F I L E S		  #


OBJ_DIR				=	obj/
OBJ_FILES			=	$(SRC_FILES:.c=.o)
OBJ					=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))


#		- G  3  O B J .  F I L E S	  #


G3_OBJ_DIR			=	g3_obj/
G3_OBJ_FILES		=	$(SRC_FILES:.c=.o)
G3_OBJ				=	$(addprefix $(G3_OBJ_DIR), $(G3_OBJ_FILES))


#######################################
#				R U L E S			  #
#######################################


#		  B U I L D  R U L E S		  #


all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION COMPLETED]\033[0m\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p obj/
	$(CC) -c $< -o $@


#	  - G 3  B U I L D  R U L E S	  #


G3_CC = clang $(INC) -Wall -Wextra -g3

# With -g3:
g: fclean $(G3_OBJ)
	$(G3_CC) $(G3_OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION WITH -G3 COMPLETED]\033[0m\n"

# With -g3 & -fsanitize:
gf: fclean $(G3_OBJ)
	$(G3_CC) $(MEM) $(G3_OBJ) $(READ) -o $(NAME)
	@echo "\n\033[32m[COMPILATION WITH -G3 & -FSANITIZE COMPLETED]\033[0m\n"

$(G3_OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p g3_obj/
	$(G3_CC) -c $< -o $@


# C L E A N  &  O T H E R  R U L E S  #


RM = rm -rf

clean:
	$(RM) $(OBJ_DIR) $(G3_OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
