/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/28 10:04:32 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_convert_envp_into_linked_list(char *envp[], t_ms *env)
{
	size_t		i;
	size_t		max;
	t_env_lst	*new;

	i = 0;
	while (envp[i])
		++i;
	max = i;
	env->envp_lst = ms_lst_create_new_node(env, envp[0]);
	i = 1;
	while (i < max)
	{
		new = ms_lst_create_new_node(env, envp[i]);
		ms_lst_add_back(env->envp_lst, new);
		++i;
	}
}

void	ms_launch_prompt(t_ms *env)

{
	char	*read_line;
	int		err_code;

	read_line = NULL;
	err_code = ms_prompt_and_execute_cmd_line_with_pipex(env, read_line);
	if (err_code == MS_ERROR)
		return ;
	err_code = ms_parse_cmd_line(env, &env->cmd_line);
	if (err_code != MS_SUCCESS)
	{
		ms_print_error_message(err_code);
		env->cmd_line = ms_free(env->cmd_line);
		return ;
	}
	ms_execute_cmdline_with_pipex(env, env->split_cmd_line);
	ppx_free_all_allocated_variables(&env->ppx_env);
	ppx_free_array_of_pointers(&env->split_cmd_line, MS_ALL);
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

	ms_handle_signals();
	ms_memset(&env, 0, sizeof(t_ms));
	if (ms_check_arguments(envp, argc) == MS_OK)
	{
		ms_convert_envp_into_linked_list(envp, &env);
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
