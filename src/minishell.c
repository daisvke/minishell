/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/20 02:25:11 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_launch_prompt(t_ms *env)
{
	ms_prompt_and_execute_cmd_line_with_pipex(env);
	ppx_free_all_allocated_variables(&env->ppx_env);
	ppx_free_array_of_pointers(env->split_cmd_line, MS_ALL);
}

void	ms_run_command_and_quit(int argc, char *argv[], t_ms *env)
{
	env->cmd_line = ms_convert_array_of_str_to_str(env, argc, argv);
	if (ms_parse_cmd_line(env, &env->cmd_line) == 1)
		return ;
	ms_free_all_allocated_variables(env);
	ms_execute_cmdline_with_pipex(env, env->split_cmd_line);
	return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	env;

	ms_init_env(envp, &env);
	ms_handle_signals();
	if (ms_check_arguments(envp) == MS_OK)
	{
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			if (argc == 1)
				ms_launch_prompt(&env);
			else
			{
				ms_run_command_and_quit(argc, argv, &env);
				break ;
			}
		}
	}
	exit(EXIT_SUCCESS);
}
