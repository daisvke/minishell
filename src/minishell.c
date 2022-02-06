/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/06 06:35:56 by dtanigaw         ###   ########.fr       */
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
	err_code = ms_run_readline(env, read_line);
	if (err_code == MS_ERROR)
		return ;
	err_code = ms_parse_cmdline(env, &env->cmdline);
	if (err_code != MS_SUCCESS)
	{
		if (err_code != 4)
			ms_print_error_message(err_code);
		env->cmdline = ms_free(env->cmdline);
		return ;
	}
	ms_execute_cmdline_with_pipex(env, env->split_cmdline);
	env->options = 0;
	ppx_free_all_allocated_variables(&env->ppx_env);
	ppx_free_array_of_pointers(&env->split_cmdline, MS_ALL);
}

void	ms_run_command_and_quit(int argc, char *argv[], t_ms *env)
{
	env->cmdline = ms_convert_array_of_str_to_str(env, argc, argv);
	if (ms_parse_cmdline(env, &env->cmdline) == 1)
		return ;
	ms_free_all_allocated_variables(env);
	ms_execute_cmdline_with_pipex(env, env->split_cmdline);
	return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	env;

	ms_handle_signals();
	ms_memset(&env, 0, sizeof(t_ms));
	if (ms_check_arguments(envp, argc) == MS_OK)
	{
		ms_init_env(envp, &env);
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
	return (0);
}
