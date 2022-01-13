/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/02 10:02:28 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmd_line_with_pipex(t_ms *env, char **cmd_line)
{
	size_t	i;
	size_t	cmd_and_file_nbr;

	i = 0;
	while (cmd_line[i])
		++i;
	cmd_and_file_nbr = i;
	ppx_init_ppx(env, &env->ppx_env, cmd_and_file_nbr);
	ppx_pipex(env, &env->ppx_env, cmd_line);
}

bool	ms_check_if_the_cmd_is_implemented(char **cmd_line, size_t *cmd_code, bool process)
{
	*cmd_code = 0;
	if (process == PPX_PROC_PARENT)
	{
		if (ms_strcmp(cmd_line[0], "cd") == MS_SAME)
			*cmd_code = MS_CMD_CD;
		else if (ms_strcmp(cmd_line[0], "exit") == MS_SAME)
			*cmd_code = MS_CMD_EXIT;
		else if (ms_strcmp(cmd_line[0], "export") == MS_SAME)
			*cmd_code = MS_CMD_EXPORT;
		else if (ms_strcmp(cmd_line[0], "unset") == MS_SAME)
			*cmd_code = MS_CMD_UNSET;
	}
	else if (process == PPX_PROC_CHILD)
	{
		if (ms_strcmp(cmd_line[0], "pwd") == MS_SAME)
			*cmd_code = MS_CMD_PWD;
		else if (ms_strcmp(cmd_line[0], "echo") == MS_SAME)
			*cmd_code = MS_CMD_ECHO;
		else if (ms_strcmp(cmd_line[0], "env") == MS_SAME)
			*cmd_code = MS_CMD_ENV;
	}
	return (*cmd_code);
}

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
		printf("minishell: envp is set to NULL\n");
		exit(EXIT_FAILURE);
	}
	if (argc > 1)
		ms_use_argv_to_create_cmd_line(argc, argv, env);
	return (MS_OK);
}

int	ms_parse_cmd_line(t_ms *env, char *cmd_line)
{
	size_t	len;

	cmd_line = ms_expand_variables(env, cmd_line);
	if (cmd_line == NULL \
		|| cmd_line[0] == '\0')
		return (1);
	len = ms_strlen(env->cmd_line);
	if (env->cmd_line[0] == '|' \
	    || env->cmd_line[len - 1] == '|')
	{
		ms_print_error_message(2);
		return (MS_ERROR);
	}
	env->split_cmd_line = ms_split_and_activate_options(env, cmd_line, '|');
	if (env->split_cmd_line == NULL)
		ms_exit_with_error_message(env, 7);
	return (MS_SUCCESS);
}

int	ms_show_prompt_and_read_cmd_line(char **cmd_line)
{

	*cmd_line = readline("\033[0;32m$\033[0;37m ");
	if (*cmd_line == NULL)
		return (MS_READ_EOF);
	if (*cmd_line[0] == '\0')
		return (MS_READ_NONE);
	return (MS_READ_LINE);
}

int	ms_prompt_and_execute_cmd_line_with_pipex(t_ms *env)
{
	size_t	res;

	env->cmd_line = ms_free(env->cmd_line);
	res = ms_show_prompt_and_read_cmd_line(&env->cmd_line);
	if (res == MS_READ_EOF)
		exit(EXIT_SUCCESS);
	if (res == MS_READ_NONE \
		|| ms_parse_cmd_line(env, env->cmd_line) == 1)
		return (1);
	ms_execute_cmd_line_with_pipex(env, env->split_cmd_line);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	env;

	ms_init_env(envp, &env);
	if (ms_check_arguments(argc, argv, envp, &env) == MS_OK)
	{
		ms_handle_signals(&env);
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			if (argc == 1)
			{
				if (ms_prompt_and_execute_cmd_line_with_pipex(&env) == 1)
					continue ;
			}
			else
			{
				ms_execute_cmd_line_with_pipex(&env, env.split_cmd_line);
				break ;
			}
		}
	}
	exit(EXIT_SUCCESS);
}
