
#                /$$           /$$           /$$                 /$$ /$$
#               |__/          |__/          | $$                | $$| $$
#  /$$$$$$/$$$$  /$$ /$$$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$
# | $$_  $$_  $$| $$| $$__  $$| $$ /$$_____/| $$__  $$ /$$__  $$| $$| $$
# | $$ \ $$ \ $$| $$| $$  \ $$| $$|  $$$$$$ | $$  \ $$| $$$$$$$$| $$| $$
# | $$ | $$ | $$| $$| $$  | $$| $$ \____  $$| $$  | $$| $$_____/| $$| $$
# | $$ | $$ | $$| $$| $$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$
# |__/ |__/ |__/|__/|__/  |__/|__/|_______/ |__/  |__/ \_______/|__/|__/
#                                                                      
#
# make		=> make with CFLAGS
# make g	=> make with -g3 without -Werror
# make gf	=> make with -g3 and fsanitize, without -Werror
#


#	 E X E C U T A B L E  N A M E	 #


NAME				=	minishell


#			C C  F L A G S			  #


CC					=	clang

CFLAGS				=	-Wall -Wextra -Werror
LDFLAGS				=	-lreadline	
INC					=	-I inc/


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
						ms_init_env_utils_1.c \
						ms_init_env_utils_2.c \
						path.c \
						pipex.c \
						pipex_utils.c \
						pipex_waitpid.c \
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
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJ)
	@echo "\n\033[32m[COMPILATION COMPLETED]\033[0m\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p obj/
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<


#	  - G 3  B U I L D  R U L E S	  #


G3_CFLAGS = -Wall -Wextra -g3

# With -g3:

g: fclean $(G3_OBJ)
	$(CC) $(LDFLAGS) -o $(NAME) $(G3_OBJ)
	@echo "\n\033[32m[COMPILATION WITH -G3 COMPLETED]\033[0m\n"

# With -g3 & -fsanitize:

MEM = -fsanitize=address -fsanitize=undefined

gf: fclean $(G3_OBJ)
	$(CC) $(LDFLAGS) $(MEM) -o $(NAME) $(G3_OBJ)
	@echo "\n\033[32m[COMPILATION WITH -G3 & -FSANITIZE COMPLETED]\033[0m\n"

# objs for all -G3 build rules

$(G3_OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p g3_obj/
	$(CC) $(G3_CFLAGS) $(INC) -o $@ -c $<


# C L E A N  &  O T H E R  R U L E S  #


RM = rm -rf

clean:
	$(RM) $(OBJ_DIR) $(G3_OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all g gf clean fclean re
