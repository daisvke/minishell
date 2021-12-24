/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 02:26:06 by dtanigaw          #+#    #+#             */
/*   Updated: 2021/12/24 03:51:26 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "minishell.h"

void	ppx_init_pipe_fds(t_ppx *env)
{
	int	size;
	int	i;

	size = env->cmd_nbr;
	env->pipe_fds = malloc(sizeof(int *) * size);
	i = 0;
	while (i < size)
	{
		env->pipe_fds[i] = malloc(sizeof(int) * 2);
		env->pipe_fds[i][0] = 0;
		env->pipe_fds[i][1] = 0;
		++i;
	}
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	str = s;
	i = 0;
	while (i < n)
		str[i++] = (unsigned char)c;
	return (s);
}

void	ppx_init_ppx(int argc, char *argv[], t_ppx *env, t_ms *ms_env)
{
	ft_memset(env, 0, sizeof(t_ppx));
	env->argc = argc;
	env->pipe = ms_env->pipe;
	if (ppx_strncmp(argv[1], "here_doc", 8) == SAME)
	{
		env->heredoc = true;
		++env->pos;
	}
	env->cmd_nbr = env->argc;
//	env->cmd_nbr = env->argc - 3 - env->pos;
	ppx_init_pipe_fds(env);
}

int	ppx_main(int argc, char *argv[], char *envp[], t_ms *ms_env)
{
	t_ppx	env;
	int		res;

//	if (argc < 5)
//		ppx_exit_with_error_message(&env, 0);
	ppx_init_ppx(argc, argv, &env, ms_env);
	res = ppx_pipex(argv, envp, &env, ms_env);
	return (SUCCESS);
}
