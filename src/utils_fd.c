/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 13:12:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/08 07:23:00 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ppx_open_file(t_ppx *env, char *file_name, int flags, int mod)
{
	int		fd;
	char	*err_message;

	if (mod)
		fd = open(file_name, flags, mod);
	else
		fd = open(file_name, flags);
	if (fd == ERROR)
	{
		err_message = strerror(errno);
		ppx_putstr_fd("pipex: ", STDERR_FILENO, NONE);
		ppx_putstr_fd(err_message, STDERR_FILENO, NONE);
		ppx_putstr_fd(": ", STDERR_FILENO, NONE);
		ppx_putstr_fd(file_name, STDERR_FILENO, PUT_NEWLINE);
		ppx_free_pipe_fds(env);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

char	**ppx_del_redirection_section_at_i(t_ppx *env, size_t del_pos)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	**new_argv;

	i = 0;
	len = 1;
	while (env->cmd[len])
		++len;
	len -= MS_REDIRECTION_SECTION;
	new_argv = malloc(sizeof(char *) * (len + 1));
	new_argv[len] = NULL;
	i = 0;
	j = 0;
	while (env->cmd[i])
	{
		if (i != del_pos && i != del_pos + 1)
		{
			new_argv[j] = ms_strdup(env->cmd[i], ppx_strlen(env->cmd[i]));
			++j;
		}
		++i;
	}
	return (new_argv);
}

void	ppx_check_redirection_arg(t_ppx *env, char *argv[], size_t i)
{
	if (argv[i + 1] == NULL)
	{
		printf("syntax error near unexpected token `newline'\n");
		exit(EXIT_FAILURE);
	}
}

bool	ppx_check_if_there_is_any_redirection(t_ppx *env)
{
	int	fd;
	size_t	i;
	int	open_flags;

	fd = 0;
	
	i = 0;
	while (env->cmd[i])
	{
		if (env->cmd[i][0] == '<')
		{
			ppx_check_redirection_arg(env, env->cmd, i);
			env->options |= MS_OPT_READ_FROM_FILE;
			fd = ppx_open_file(env, env->cmd[env->pos + 1], O_RDONLY, 0);
			ppx_dup2(env, fd, 0);
			env->cmd = ppx_del_redirection_section_at_i(env, i);
			return (true);
		}
		else if (env->cmd[i][0] == '>')
		{
			ppx_check_redirection_arg(env, env->cmd, i);
			env->options |= MS_OPT_REDIR_OUTPUT;
			open_flags = ppx_get_open_flags(env);
			fd = ppx_open_file(env, env->cmd[env->pos + 1], open_flags, 0664);
			ppx_dup2(env, fd, 1);
			env->cmd = ppx_del_redirection_section_at_i(env, i);
			return (true);
		}
		++i;
	}
	return (false);
}

void	ppx_get_fd(t_ppx *env, char *argv[])
{
	int	fd;
//	int	open_flags;

	fd = 0;
//	open_flags = ppx_get_open_flags(env);
	if ((env->options & MS_OPT_HEREDOC) \
		&& env->pos == FIRST_CMD_WHEN_HEREDOC)
	{
		fd = ppx_open_file(env, "heredoc_output", O_RDONLY, 0);
		ppx_dup2(env, fd, 0);
	}
	/*
	else if ((env->options & MS_OPT_REDIR_OUTPUT) \
		&& env->pos == FIRST_CMD_WHEN_READING_FROM_FILE)
	{
		//get fd on next arg
		fd = ppx_open_file(env, argv[INPUT_FILE], O_RDONLY, 0);
		ppx_dup2(env, fd, 0);
	}*/
	if ((env->options & MS_OPT_REDIR_OUTPUT) == false  \
		&& env->pos == env->argc - 1)
	{
		ppx_dup2(env, STDOUT_FILENO, 1);
	}
	/*
	else if ((env->options & MS_OPT_REDIR_OUTPUT) \
		&& env->pos == env->argc - GET_LAST_CMD)
	{
		fd = ppx_open_file(env, argv[env->argc - 1], open_flags, 0664);
		ppx_dup2(env, fd, 1);
	}*/
}

void	ppx_putstr_fd(char *s, int fd, bool option)
{
	if (s)
	{
		write(fd, s, ppx_strlen(s));
		if (option == PUT_NEWLINE)
			write(fd, "\n", 1);
	}
}
