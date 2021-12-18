/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/16 04:57:27 by dtanigaw         ###   ########.fr       */
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

int	ms_check_if_cmd_is_handled(char *cmd)
{
	if (ms_strncmp(cmd, "echo ", 5) == MS_SAME \
		|| ms_strncmp(cmd, "pwd", 3) == MS_SAME \
		|| ms_strncmp(cmd, "env", 3) == MS_SAME)
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

int	main(int argc, char *argv[], char *envp[])
{
	char	*buffer;

	if (ms_check_if_args_are_set(argc, argv) == MS_NO_ARGS)
	{
		while (1)
		{
			buffer = readline("$ ");
	//		printf("str: %s\n", buffer);
			//if (ms_check_if_cmd_is_handled(buffer) == true)
				ms_execute_cmd(envp, buffer);
		}
	}
	exit(EXIT_SUCCESS);
}
