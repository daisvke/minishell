/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 22:05:04 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_lst	g_envp;

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

void	ms_prompt_and_execute_cmd_line_with_pipex(char *envp[], t_ms *env)
{
	size_t	res;
	char	*read_line;

	res = ms_show_prompt_and_read_cmd_line(&read_line);
	if (res == MS_READ_EOF)
		exit(EXIT_SUCCESS);
	ms_init_env(envp, env, true);
	env->cmd_line = read_line;
	if (res == MS_READ_NONE \
		|| ms_parse_cmd_line(env, env->cmd_line) == 1)
		return ;
	ms_execute_cmd_line_with_pipex(env, env->split_cmd_line);
}

size_t	ms_count_char_in_array(char *argv[])
{
	size_t	i;
	size_t	j;
	size_t	res;

	res = 0;
	i = MS_FIRST_ARG_POS;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			++res;
			++j;
		}
		++res;
		++i;
	}
	return (res);
}

char	*ms_convert_array_of_str_to_str(t_ms *env, int argc, char *argv[])
{
	size_t	len;
	int		i;
	size_t	j;
	char	*str;

	len = ms_count_char_in_array(argv);
	str = ms_malloc(env, len, sizeof(char));
	ms_memset(str, 0, len);
	i = MS_FIRST_ARG_POS;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] != ' ')
			{
				*str = argv[i][j];
	printf("len: %c\n",*(str));
				str++;
			}
			++j;
		}
		if (i < argc - 1)
		{
			*str = ' ';
	printf("len: %c\n",*(str));
			str++;
		}
		++i;
	}
	*str = '\0';
	return (str);
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
	if (ms_check_arguments(argc, argv, envp, &env) == MS_OK)
	{
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			if (argc == 1)
			{
				ms_prompt_and_execute_cmd_line_with_pipex(envp, &env);
				last_pipe_exit_status = env.last_pipe_exit_status;
				ms_free_all_allocated_variables(&env);
				env.last_pipe_exit_status = last_pipe_exit_status;
			}
			else
			{
				env.cmd_line = ms_convert_array_of_str_to_str(&env, argc, argv);
				if (ms_parse_cmd_line(&env, env.cmd_line) == 1)
					break ;
				ms_execute_cmd_line_with_pipex(&env, env.split_cmd_line);
				ms_free_all_allocated_variables(&env);
				break ;
			}
		}
	}
	exit(EXIT_SUCCESS);
}
