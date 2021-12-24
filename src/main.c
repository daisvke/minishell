/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/25 00:45:57 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmd(char *envp[], t_ms *ms_env, char **cmd_line)
{
	t_ppx	ppx_env;
	char	*path_to_cmd;
	size_t	i;
	size_t	cmd_and_file_nbr;

	i = 0;
	while (cmd_line[i])
		++i;
	cmd_and_file_nbr = i;
	ppx_main(cmd_and_file_nbr, cmd_line, envp, ms_env);
}

int	s_check_if_the_cmd_exists_inside_minishell(char *cmd_line)
{
	if (\
		ms_strncmp(cmd_line, "cd", 2) == MS_SAME \
		|| ms_strncmp(cmd_line, "export", 6) == MS_SAME \
		|| ms_strncmp(cmd_line, "unset", 5) == MS_SAME \
		|| ms_strncmp(cmd_line, "exit", 4) == MS_SAME \
		)
		return (true);
	return (false);
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
