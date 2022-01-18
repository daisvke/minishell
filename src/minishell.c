/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/19 00:08:56 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_lst	g_envp;

int	ms_launch_prompt(char *envp[], t_ms *env, int last_pipe_exit_status)
{
	ms_prompt_and_execute_cmd_line_with_pipex(envp, env, last_pipe_exit_status);
	last_pipe_exit_status = env->last_pipe_exit_status;
	ms_free_all_allocated_variables(env);
	return (last_pipe_exit_status);
}

void	ms_run_command_and_quit(int argc, char *argv[], t_ms *env)
{
	env->cmd_line = ms_convert_array_of_str_to_str(env, argc, argv);
	if (ms_parse_cmd_line(env, env->cmd_line) == 1)
		return ;
	ms_execute_cmdline_with_pipex(env, env->split_cmd_line);
	ms_free_all_allocated_variables(env);
	return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	env;
	int		last_pipe_exit_status;
	
	if (argc == 1)
		ms_init_env(envp, &env, false);
	else
		ms_init_env(envp, &env, true);
	ms_handle_signals();
	if (ms_check_arguments(envp, &env) == MS_OK)
	{
		last_pipe_exit_status = 0;
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			if (argc == 1)
				last_pipe_exit_status = ms_launch_prompt(\
					envp, &env, last_pipe_exit_status);
			else
			{
				ms_run_command_and_quit(argc, argv, &env);
				break ;
			}
		}
	}
	exit(EXIT_SUCCESS);
}
