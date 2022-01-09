/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 13:12:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/09 21:35:21 by dtanigaw         ###   ########.fr       */
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

char	**ppx_del_redirection_section_at_i(t_ppx *env, size_t del_line, size_t del_pos, size_t lines_to_del)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	**new_argv;

	i = 0;
	len = 1;
	while (env->cmd[len])
		++len;
	if (len == lines_to_del)
		len += 1;
	len -= lines_to_del;
	new_argv = malloc(sizeof(char *) * (len + 1));
	new_argv[len] = NULL;
	i = 0;
	j = 0;
	while (env->cmd[i])
	{
		if (del_line == 0 && env->cmd[0][0] != '<' && env->cmd[0][0] != '>' && env->cmd[0][1] != '\0')
		{
			new_argv[j] = ms_strdup(env->cmd[0], del_pos);
		}
		else if ((lines_to_del == 1 && i != del_line) \
			|| (lines_to_del == 2 && i != del_line && i != del_line + 1))
		{
			new_argv[j] = ms_strdup(env->cmd[i], ppx_strlen(env->cmd[i]));
			++j;
		}
		++i;
	}
	if (*new_argv[0] == '\0')
	{
		free(new_argv);
		new_argv = NULL;
	}
	return (new_argv);
}

char	*ppx_get_redirection_out_file(t_ppx *env, char *cmd[], size_t i, size_t *lines_to_del)
{
	char	*file;

	*lines_to_del = 1;
	if (cmd[i][1] == '\0')
	{
		if (cmd[i + 1] == NULL)
		{
			printf("syntax error near unexpected token `newline'\n");
			exit(EXIT_FAILURE);
		}
		*lines_to_del = 2;
		return (cmd[i + 1]);
	}
	else if (cmd[0][0] != '<' && cmd[0][0] != '>' && ms_strchr(cmd[i], "<>"))
	{
		file = ms_strchr(cmd[i], "<>");
		return (file + 1);
	}
	else
		return (&cmd[i][1]);
}

void	ppx_apply_redirection(t_ppx *env, char c, char *file)
{
	int	fd;
	int	open_flags;

	fd = 0;
	if (c == '<')
	{
		env->options |= MS_OPT_READ_FROM_FILE;
		fd = ppx_open_file(env, file, O_RDONLY, 0);
		ppx_dup2(env, fd, STDIN_FILENO);
	}
	else if (c == '>')
	{
		env->options |= MS_OPT_REDIR_OUTPUT;
		open_flags = ppx_get_open_flags(env);
		fd = ppx_open_file(env, file, open_flags, 0664);
		ppx_dup2(env, fd, STDOUT_FILENO);
	}
}

void	ppx_handle_redirections(t_ppx *env)
{
	size_t	i;
	size_t	j;
	char	*file;
	size_t	lines_to_del;

	i = 0;
	while (env->cmd[i])
	{
		j = 0;
		while (env->cmd[i][j])
		{
	//		printf("cmd: %c\n", env->cmd[i][j]);
			if (env->cmd[i][j] == '<' || env->cmd[i][j] == '>')
			{
				//open stdin if >t and nothing before/after or execve cat
				file = ppx_get_redirection_out_file(env, env->cmd, i, &lines_to_del);
				ppx_apply_redirection(env, env->cmd[i][j], file);
				env->cmd = ppx_del_redirection_section_at_i(env, i, j, lines_to_del);
				i = 0;
				j = 0;
			}
			else
				++j;
		}
		++i;
	}
}

void	ppx_get_fd(t_ppx *env, char *argv[])
{
	int	fd;

	fd = 0;
	if ((env->options & MS_OPT_HEREDOC) \
		&& env->pos == FIRST_CMD_WHEN_HEREDOC)
	{
		fd = ppx_open_file(env, "heredoc_output", O_RDONLY, 0);
		ppx_dup2(env, fd, 0);
	}
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
