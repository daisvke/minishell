#include "minishell.h"

void	ms_use_argv_to_create_cmd_line(int argc, char *argv[], t_ms *env)
{
	int		i;
	size_t	j;

	env->split_cmd_line = ms_malloc(env, argc, sizeof(char *)); 
	i = MS_FIRST_ARG_POS;
	j = 0;
	while (i < argc)
	{
		env->split_cmd_line[j] = ms_strdup(argv[i], ms_strlen(argv[i]));
		++j;
		++i;
	}
	env->split_cmd_line[j] = NULL;
}

int	ms_check_arguments(int argc, char *argv[], char *envp[], t_ms *env)
{
	(void)argv;
	if (envp == NULL)
	{
		ms_print_error_message(4);
		exit(EXIT_FAILURE);
	}
	if (argc > 1)
		ms_use_argv_to_create_cmd_line(argc, argv, env);
	return (MS_OK);
}

size_t	ms_get_symbol_error_code(char symbol)
{
	if (symbol == '|')
		return (2);
	if (symbol == '<')
		return (3);
	else
		return (4);
}

int	ms_check_pipes_and_redirections(t_ms *env, char *cmd_line)
{
	size_t	len;
	char	symbol;
	size_t	i;

	len = ms_strlen(env->cmd_line);
	if (env->cmd_line[0] == '|' \
	    || env->cmd_line[len - 1] == '|')
		return (2);
	while (*cmd_line)
	{
		if (*cmd_line == '|' || *cmd_line == '<' ||*cmd_line == '>')
		{
			i = 0;
			symbol = *cmd_line;
			while (cmd_line && *cmd_line == symbol)
			{
				cmd_line++;
				++i;
			}
			if ((symbol == '|' && i > 1) || (symbol == '<' && i > 3) || (symbol == '>' && i > 2))
				return (ms_get_symbol_error_code(symbol));
		}
		cmd_line++;
	}
	return (MS_SUCCESS);
}

int	ms_parse_cmd_line(t_ms *env, char *cmd_line)
{
	int	err_code;

	err_code = ms_check_pipes_and_redirections(env, cmd_line);
	if (err_code != MS_SUCCESS)
	{
		ms_print_error_message(err_code);
		return (MS_ERROR);
	}
	cmd_line = ms_expand_variables(env, cmd_line);
	if (cmd_line == NULL \
		|| cmd_line[0] == '\0')
		return (1);
	env->split_cmd_line = ms_split_and_activate_options(env, cmd_line, '|');
	if (env->split_cmd_line == NULL)
		ms_exit_with_error_message(env, 7);
	return (MS_SUCCESS);
}
