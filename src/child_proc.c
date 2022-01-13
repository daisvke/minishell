#include "minishell.h"

void	ppx_spawn_child_to_execute_cmd(t_ms *ms_env, t_ppx *ppx_env)
{
	size_t	cmd_code;
	char	*path_to_cmd;
	size_t	lst_size;
	char	**envp;
	
	ppx_handle_redirections(ppx_env);
	if (ppx_env->cmd)
	{
		if (ppx_env->options & MS_OPT_PIPE)
		{
			ppx_close(ppx_env, ppx_env->pipe_fds[ppx_env->i][0]);
			ppx_dup2(ppx_env, ppx_env->fd_in, STDIN_FILENO);
			if (ppx_env->pos != ppx_env->cmd_nbr - 1 \
				&& (ms_env->options & MS_OPT_REDIR_OUTPUT) == false)
				ppx_dup2(ppx_env, ppx_env->pipe_fds[ppx_env->i][1], STDOUT_FILENO);
	//		ppx_get_fd(ppx_env, argv);
		}
		if (ms_check_if_the_cmd_is_implemented(ppx_env->cmd, &cmd_code, PPX_PROC_CHILD) == true)
			ppx_execute_implemented_cmd(ms_env, ppx_env, cmd_code, ppx_env->cmd);
		else
		{
			path_to_cmd = ppx_get_the_right_cmd_path(ms_env, ppx_env, "PATH=", ppx_env->cmd[0]);
			lst_size = ms_lst_lstsize(ms_env->envp_lst);
			envp = ms_convert_envp_lst_to_array_of_pointers(ms_env->envp_lst, lst_size);
			if (execve(path_to_cmd, ppx_env->cmd, envp) == PPX_ERROR)
				ppx_exit_when_cmd_not_found(ppx_env, ppx_env->cmd[0]);
			//no exit+error handling 
			ppx_free_array_of_pointers(envp, lst_size);
		}
	}
}

