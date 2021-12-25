/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 07:28:58 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmd(char *envp[], t_ms *ms_env, char **cmd_line)
{
	size_t	i;
	size_t	cmd_and_file_nbr;

	i = 0;
	while (cmd_line[i])
		++i;
	cmd_and_file_nbr = i;
	ppx_main(cmd_and_file_nbr, cmd_line, envp, ms_env);
}
// add env for err
void	ms_add_curr_path_to_ls_cmd(t_ppx *env, char **cmd_and_args)
{
	size_t	i;
	char	*curr_abs_path;

	curr_abs_path = getcwd(NULL, 0);
//	if (curr_abs_path == NULL)
	//	ppx_exit_with_error_message(env, 10);
	i = MS_FIRST_ARG_POS;
	if (cmd_and_args[i] == NULL)
	{
//		strcat(curr_abs_path, cmd_and_args[i]);
		cmd_and_args[i] = ppx_join_two_str(env, curr_abs_path, cmd_and_args[i]);
		printf("strcat: %s\n", cmd_and_args[i]);
		return ;
	}
	while (cmd_and_args[i])
	{
		cmd_and_args[i] = ppx_join_two_str(env, curr_abs_path, cmd_and_args[i]);
	//	strcat(curr_abs_path, cmd_and_args[i]);
		printf("strcat: %s\n", cmd_and_args[i]);
		++i;
	}
		// ms_free
}

bool	ms_check_if_the_cmd_is_implemented(t_ppx *env, char **cmd_line, size_t *cmd_code)
{
	*cmd_code = 0;
	if (ms_strcmp(cmd_line[0], "cd") == MS_SAME)
	{
		*cmd_code = MS_CMD_CD;
	}
	else if (ms_strcmp(cmd_line[0], "export") == MS_SAME)
		*cmd_code = MS_CMD_EXPORT;
	else if (ms_strcmp(cmd_line[0], "unset") == MS_SAME)
		*cmd_code = MS_CMD_UNSET;
	else if (ms_strcmp(cmd_line[0], "exit") == MS_SAME)
		*cmd_code = MS_CMD_EXIT;
	else if (ms_strcmp(cmd_line[0], "ls") == MS_SAME)
		ms_add_curr_path_to_ls_cmd(env, cmd_line);
	return (*cmd_code);
}

bool	ms_check_if_args_are_set(int argc)
{
	if (argc > 1)
	{
		return (MS_ERROR);
	}
	return (MS_NO_ARGS);
}

void	ms_parse_cmd_line(t_ms *ms_env, char *cmd_line)
{
	ms_env->cmd_line = ms_split_and_activate_options(ms_env, cmd_line, '|');
	if (ms_env->cmd_line == NULL)
	{
		//set error
		//exit
	}
}

int	ms_show_prompt_and_read_cmd_line(char **cmd_line)
{
	*cmd_line = readline("$ ");
	if (*cmd_line == NULL)
		return (MS_EMPTY_CMD_LINE);
	add_history(*cmd_line);
	return (MS_LINE_READ);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	ms_env;
	char	*cmd_line;

	ft_memset(&ms_env, 0, sizeof(t_ms));
	(void)argv;

//	size_t	i = 0;
	if (ms_check_if_args_are_set(argc) == MS_NO_ARGS)
	{
		cmd_line = NULL;
		while (1)
		{
			if (ms_show_prompt_and_read_cmd_line(&cmd_line) == MS_EMPTY_CMD_LINE)
				continue ;
			ms_parse_cmd_line(&ms_env, cmd_line);
	//		if (ms_check_if_the_cmd_exists_inside_minishell(cmd_line) == true)
	/*
			for (i = 0; ms_env.cmd_line[i]; ++i)
				printf("str: %s\n", ms_env.cmd_line[i]);
	*/
			ms_execute_cmd(envp, &ms_env, ms_env.cmd_line);
			free(cmd_line);
		}
	}
	// ms_free
	exit(EXIT_SUCCESS);
}
