
#include "minishell.h"

void	ppx_close(t_ppx *env, int fd)
{
	if (close(fd) == ERROR)
		ppx_exit_with_error_message(env, 5);
}

void	ppx_dup2(t_ppx *env, int fd1, int fd2)
{
	if (dup2(fd1, fd2) == ERROR)
		ppx_exit_with_error_message(env, 4);
}

void	ppx_pipe(t_ppx *env, int *fds)
{
	if (pipe(fds) == ERROR)
		ppx_exit_with_error_message(env, 3);
}

pid_t	ppx_fork(t_ppx *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		ppx_exit_with_error_message(env, 2);
	return (pid);
}

