/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/21 07:00:55 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmd(char *envp[], char *read_line)
{
	t_ppx	ppx_env;
	char	*path_to_cmd;

	ppx_env.cmd = ppx_split(read_line, ' ');
	path_to_cmd = ppx_get_the_right_cmd_path(&ppx_env, envp, "PATH=", ppx_env.cmd[0]);
	if (execve(path_to_cmd, ppx_env.cmd, envp) == ERROR)
		ppx_exit_when_cmd_not_found(&ppx_env, ppx_env.cmd[0]);
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

//void	ms_()

bool	ms_check_if_args_are_set(int argc, char *argv[])
{
	if (argc > 1)
	{
		
		return (MS_ERROR);
	}
	return (MS_NO_ARGS);
}

void	ms_parse_cmd_line(t_ms *ms_env, char *cmd_line)
{
	size_t	start;
	size_t	end;

	ms_env->array_of_cmds = ppx_split(cmd_line, '|');
	if (array_of_cmds == NULL)
	{
		//set error
		//exit
	}
}

int	ms_show_prompt_and_read_cmd_line(char *cmd_line)
{
	cmd_line = readline("$ ");
	if (cmd_line == NULL)
		return (MS_EMPTY_CMD_LINE);
	return (MS_LINE_READ);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	ms_env;
	char	*cmd_line;

	if (ms_check_if_args_are_set(argc, argv) == MS_NO_ARGS)
	{
		while (1)
		{
			if (ms_show_prompt_and_read_cmd_line(cmd_line) == MS_EMPTY_CMD_LINE)
				continue ;
			ms_parse_cmd_line(&ms_env, cmd_line);
	//		printf("str: %s\n", buffer);
	//		if (ms_check_if_the_cmd_exists_inside_minishell(cmd_line) == true)
				
			ms_execute_cmd(envp, cmd_line);
		}
	}
	// ms_free
	free(cmd_line);
	exit(EXIT_SUCCESS);
}
