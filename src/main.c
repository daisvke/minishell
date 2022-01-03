/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 03:24:27 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/02 10:02:28 by dtanigaw         ###   ########.fr       */
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
	while (cmd_and_args[i])
	{
		cmd_and_args[i] = ppx_join_three_str(env, curr_abs_path, "/", cmd_and_args[i]);
		printf("strcat: %s\n", cmd_and_args[i]);
		++i;
	}
		// ms_free
}

bool	ms_check_if_the_cmd_is_implemented(t_ppx *env, char **cmd_line, size_t *cmd_code, bool process)
{
	*cmd_code = 0;
	if (process == PPX_PROC_PARENT)
	{
		if (ms_strcmp(cmd_line[0], "cd") == MS_SAME)
			*cmd_code = MS_CMD_CD;
		else if (ms_strcmp(cmd_line[0], "exit") == MS_SAME)
			*cmd_code = MS_CMD_EXIT;
		else if (ms_strcmp(cmd_line[0], "export") == MS_SAME)
			*cmd_code = MS_CMD_EXPORT;
		else if (ms_strcmp(cmd_line[0], "unset") == MS_SAME)
			*cmd_code = MS_CMD_UNSET;
	}
	else if (process == PPX_PROC_CHILD)
	{
		if (ms_strcmp(cmd_line[0], "pwd") == MS_SAME)
			*cmd_code = MS_CMD_PWD;
		else if (ms_strcmp(cmd_line[0], "echo") == MS_SAME)
			*cmd_code = MS_CMD_ECHO;
		else if (ms_strcmp(cmd_line[0], "env") == MS_SAME)
			*cmd_code = MS_CMD_ENV;
	}
	return (*cmd_code);
}

int	ms_control_arguments(int argc, char *argv[])
{
	//check if envp is true
	(void)argv;
	if (argc > 1)
	{
		return (MS_ERROR);
	}
	return (MS_OK);
}
/*
void	ms_expand_variables(t_ms *env, char *cmd_line)
{
	while ()
}
*/
void	ms_parse_cmd_line(t_ms *env, char *cmd_line)
{
//	ms_expand_variables(env, cmd_line);
	env->split_cmd_line = ms_split_and_activate_options(env, cmd_line, '|');
	if (env->split_cmd_line == NULL)
	{
		//set error
		exit(1);
	}
}

void	ms_handle_sigint(int signum)
{
	(void)signum;

	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void	ms_handle_sigquit(int signum)
{
	struct termios orig_termios_p;
	struct termios new_termios_p;

// errno is set
	if (tcgetattr(STDIN_FILENO, &orig_termios_p) != MS_SUCCESS)
		exit(EXIT_FAILURE);
	new_termios_p = orig_termios_p;
	new_termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios_p) != MS_SUCCESS)
		exit(EXIT_FAILURE);
}

void	ms_handle_signals(t_ms *ms_env)
{
	struct sigaction	signal_action;
	struct sigaction	signal_action2;

	signal_action.sa_handler = &ms_handle_sigint;
	sigaction(SIGINT, &signal_action, NULL);
	signal_action2.sa_handler = &ms_handle_sigquit;
	sigaction(SIGQUIT, &signal_action2, NULL);
}

int	ms_show_prompt_and_read_cmd_line(t_ms *ms_env, char **cmd_line)
{
	ms_env->cmd_line = readline("\033[0;32m$\033[0;37m ");
	if (ms_env->cmd_line == NULL)
		return (MS_READ_EOF);
	if (ms_env->cmd_line[0] == '\0')
		return (MS_READ_NONE);
	return (MS_READ_LINE);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	ms_env;
	size_t	res;

	if (ms_control_arguments(argc, argv) == MS_OK)
	{
		ms_init_env(envp, &ms_env);
		ms_handle_signals(&ms_env);
		while (MS_LOOP_NOT_ENDED_BY_CTRL_D)
		{
			if (ms_env.cmd_line)
				free(ms_env.cmd_line);
			res = ms_show_prompt_and_read_cmd_line(&ms_env, &ms_env.cmd_line);
			if (res == MS_READ_EOF)
				exit(EXIT_SUCCESS);
			if (res == MS_READ_NONE)
				continue ;
			ms_parse_cmd_line(&ms_env, ms_env.cmd_line);
			ms_execute_cmd_line(ms_env.envp, &ms_env, ms_env.split_cmd_line);
			// ms_free
		}
	}
	exit(EXIT_SUCCESS);
}
