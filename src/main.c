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

void	ms_execute_cmd_line(t_ms *env, char **cmd_line)
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

int	ms_control_arguments(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc > 1 \
		|| envp == NULL)
		return (MS_ERROR);
	return (MS_OK);
}

int	ms_parse_cmd_line(t_ms *env, char *cmd_line)
{
	size_t	len;

	cmd_line = ms_expand_variables(env, cmd_line);
	if (cmd_line == NULL \
		|| cmd_line[0] == '\0')
		return (1);
	len = ppx_strlen(env->cmd_line);
	if (env->cmd_line[0] == '|' \
	    || env->cmd_line[len - 1] == '|')
	{
		printf("syntax error near unexpected token `|'\n" );
		return (MS_ERROR);
	}
	env->split_cmd_line = ms_split_and_activate_options(env, cmd_line, '|');
	if (env->split_cmd_line == NULL)
	{
		//set error
		exit(1);
	}
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

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	env;
	size_t	res;

	if (ms_control_arguments(argc, argv, envp) == MS_OK)
	{
		ms_init_env(envp, &env);
		ms_handle_signals(&env);
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			env.cmd_line = ms_free(env.cmd_line);
			res = ms_show_prompt_and_read_cmd_line(&env.cmd_line);
			if (res == MS_READ_EOF)
				exit(EXIT_SUCCESS);
			if (res == MS_READ_NONE \
				|| ms_parse_cmd_line(&env, env.cmd_line) == 1)
				continue ;
			ms_execute_cmd_line(&env, env.split_cmd_line);
			// ppx_free
		}
	}
	exit(EXIT_SUCCESS);
}
