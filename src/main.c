/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/26 01:00:40 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_execute_cmd_line(char *envp[], t_ms *ms_env, char **cmd_line)
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
	/*
	if (cmd_and_args[i] == NULL)
	{
		ms_free_split(cmd_and_args);
		//protect
		cmd_and_args = malloc(sizeof(char *) * 3);
		cmd_and_args[0] = "ls";
//		strcat(curr_abs_path, cmd_and_args[i]);
		cmd_and_args[1] = curr_abs_path;
		cmd_and_args[2] = NULL;
		//free cmd after use 
		return ;
	}
	*/
	while (cmd_and_args[i])
	{
		cmd_and_args[i] = ppx_join_three_str(env, curr_abs_path, "/", cmd_and_args[i]);
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
		*cmd_code = MS_CMD_CD;
	else if (ms_strcmp(cmd_line[0], "pwd") == MS_SAME)
		*cmd_code = MS_CMD_PWD;
	else if (ms_strcmp(cmd_line[0], "export") == MS_SAME)
		*cmd_code = MS_CMD_EXPORT;
	else if (ms_strcmp(cmd_line[0], "unset") == MS_SAME)
		*cmd_code = MS_CMD_UNSET;
	else if (ms_strcmp(cmd_line[0], "exit") == MS_SAME)
		*cmd_code = MS_CMD_EXIT;
	return (*cmd_code);
}

int	ms_control_arguments(int argc, char *argv[])
{
	(void)argv;
	if (argc > 1)
	{
		return (MS_ERROR);
	}
	return (MS_OK);
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

void	ms_handle_sigint(int signum)
{
	(void)signum;

	rl_replace_line("\n", 0);
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ms_handle_sigint);
}

void	ms_handle_signals(t_ms *ms_env)
{
	signal(SIGINT, ms_handle_sigint);
}

int	ms_show_prompt_and_read_cmd_line(t_ms *ms_env, char **cmd_line)
{
	*cmd_line = readline("$ ");
	if (*cmd_line == NULL)
		return (MS_READ_EOF);
	add_history(*cmd_line);
	return (MS_READ_LINE);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	ms_env;
	char	*cmd_line;

	if (ms_control_arguments(argc, argv) == MS_OK)
	{
		ms_init_env(&ms_env);
		cmd_line = NULL;
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			ms_handle_signals(&ms_env);
			if (ms_show_prompt_and_read_cmd_line(&ms_env, &cmd_line) == MS_READ_EOF)
				exit(EXIT_SUCCESS);
			ms_parse_cmd_line(&ms_env, cmd_line);
			ms_execute_cmd_line(envp, &ms_env, ms_env.cmd_line);
			// ms_free
			free(cmd_line);
		}
	}
	exit(EXIT_SUCCESS);
}
