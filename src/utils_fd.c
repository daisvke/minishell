/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 13:12:35 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/10 12:41:24 by dtanigaw         ###   ########.fr       */
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
	ft_memset(new_argv, 0, sizeof(char **) * (len + 1));
	i = 0;
	j = 0;
	while (env->cmd[i])
	{
		if (del_line == i \
			&& ms_check_if_char_is_a_redir_symbol(env->cmd[i][0]) == false \
			&& env->cmd[i][1] != '\0')
			new_argv[j] = ms_strdup(env->cmd[i], del_pos);
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

char	*ppx_check_outfile(t_ppx *env, char *file, size_t i, size_t j, size_t *lines_to_del)
{
	*lines_to_del = 1;
	if (*file == '\0')
	{
		if (env->cmd[i + 1] == NULL)
		{
			printf("syntax error near unexpected token `newline'\n" );
			exit(EXIT_FAILURE);
		}
		file = env->cmd[i + 1];
		*lines_to_del = 2;
		return (file);
	}
	else
		return (file);
}

void	ppx_apply_redirection(t_ppx *env, char *str, char *file)
{
	int	fd;
	int	open_flags;

	fd = 0;
	if (*str == '<' && *(str + 1) == '<')
	{
		env->options |= MS_OPT_HEREDOC;
		ppx_request_heredoc_input(env, file);
//		fd = ppx_open_file(env, "heredoc_output", O_RDONLY, 0);
//		ppx_dup2(env, fd, STDIN_FILENO);
//		if (env->options & MS_OPT_HEREDOC)
	//	unlink("heredoc_output");
	}
	else if (*str == '>' && *(str + 1) == '>')
	{
		env->options |= MS_OPT_APPEND_OUTPUT; 
		ppx_close(env, env->pipe_fds[env->i][1]);
		open_flags = ppx_get_open_flags(env);
		fd = ppx_open_file(env, file, open_flags, 0664);
		ppx_dup2(env, fd, STDOUT_FILENO);
	}
	else if (*str == '<')
	{
		env->options |= MS_OPT_READ_FROM_FILE;
		fd = ppx_open_file(env, file, O_RDONLY, 0);
		ppx_dup2(env, fd, STDIN_FILENO);
	}
	else if (*str == '>')
	{
		env->options |= MS_OPT_REDIR_OUTPUT;
		ppx_close(env, env->pipe_fds[env->i][1]);
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
			if (ms_search_redir_symbol(&env->cmd[i][j]))
			{
				file = ms_search_redir_symbol(&env->cmd[i][j]);
				file = ppx_check_outfile(env, file, i, j, &lines_to_del);
				ppx_apply_redirection(env, &env->cmd[i][j], file);
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
/*
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
*/
void	ppx_putstr_fd(char *s, int fd, bool option)
{
	if (s)
	{
		write(fd, s, ppx_strlen(s));
		if (option == PUT_NEWLINE)
			write(fd, "\n", 1);
	}
}
