/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 07:27:32 by dtanigaw         ###   ########.fr       */
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

int	ms_show_prompt_and_read_cmd_line(char **cmd_line)
{
	*cmd_line = readline("\033[0;32m$\033[0;37m ");
	if (*cmd_line == NULL)
		return (MS_READ_EOF);
	if (*cmd_line[0] == '\0')
		return (MS_READ_NONE);
	add_history(*cmd_line);
	return (MS_READ_LINE);
}

int	ms_parse_cmd_line(t_ms *env, char *cmd_line)
{
	int	err_code;

	err_code = ms_check_if_quote_nbr_is_even(cmd_line);
	if (err_code != MS_SUCCESS)
	{
		ms_print_error_message(err_code);
		return (MS_ERROR);
	}
	err_code = ms_check_pipes_and_redirections(env, cmd_line);
	if (err_code != MS_SUCCESS)
	{
		ms_print_error_message(err_code);
		return (MS_ERROR);
	}
	cmd_line = ms_expand_variables(env, cmd_line);
	if (cmd_line == NULL \
		|| cmd_line[0] == '\0')
		return (1);
	env->split_cmd_line = ms_split_and_activate_options(env, cmd_line, '|');
	if (env->split_cmd_line == NULL)
		ms_exit_with_error_message(env, 0);
	return (MS_SUCCESS);
}

void	ms_prompt_and_execute_cmd_line_with_pipex(t_ms *env)
{
	size_t	res;

	env->cmd_line = ms_free(env->cmd_line);
	res = ms_show_prompt_and_read_cmd_line(&env->cmd_line);
	if (res == MS_READ_EOF)
		exit(EXIT_SUCCESS);
	if (res == MS_READ_NONE \
		|| ms_parse_cmd_line(env, env->cmd_line) == 1)
		return ;
	ms_execute_cmd_line_with_pipex(env, env->split_cmd_line);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	env;
	int		last_pipe_exit_status;

	ms_init_env(envp, &env);
	ms_handle_signals();
	if (ms_check_arguments(argc, argv, envp, &env) == MS_OK)
	{
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			if (argc == 1)
			{
				ms_prompt_and_execute_cmd_line_with_pipex(&env); 
				last_pipe_exit_status = env.last_pipe_exit_status;
				ms_free_all_allocated_variables(&env);
				ms_init_env(envp, &env);
				env.last_pipe_exit_status = last_pipe_exit_status;
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
