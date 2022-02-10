/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 11:17:16 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/10 02:50:52 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_get_new_path_length(\
	char *home_path, char *current_path, size_t home_path_len)
{
	size_t	i;
	size_t	curr_path_len;
	int		res;

	curr_path_len = ms_strlen(current_path);
	i = 0;
	while (current_path[i] && home_path[i] && current_path[i] == home_path[i])
		++i;
	if (i == 0)
		return (curr_path_len);
	if (i == ms_strlen(home_path) && i == curr_path_len)
		return (MS_PMP_AT_HOME);
	else
		res = MS_PMP_TILDE_LEN \
			+ (curr_path_len - home_path_len) \
			+ MS_PMP_DOLLAR_SPACE_LEN;
	return (res);
}

void	ms_generate_new_path_for_prompt(\
	t_ms *env, char *current_path, int len, bool first_time)
{
	size_t	go_forward;
	char	*tilde;

	tilde = "~";
	go_forward = env->cmd_prompt.home_path_len;
	if (first_time == false)
		ms_free(env->cmd_prompt.prompt);
	if (len < 0)
	{
		go_forward = 0;
		tilde = "";
	}
	if (len == MS_PMP_AT_HOME)
	{
		env->cmd_prompt.prompt = ms_strdup(\
			"~\001\x1B[2m\x1B[37m\002$ ", 3 + 11);
		if (env->cmd_prompt.prompt == NULL)
			exit(EXIT_FAILURE);
	}
	else if (ms_strcmp(current_path, "/") == MS_SAME)
	{
		env->cmd_prompt.prompt = ppx_join_three_str(\
			&env->ppx_env, \
			"/", "\001\x1B[2m\x1B[37m\002$", " "
		);//ms vers ?
	}
	else
	{
		env->cmd_prompt.prompt = ppx_join_three_str(\
			&env->ppx_env, \
			tilde, \
			current_path + go_forward, \
			"\001\x1B[2m\x1B[37m\002$ "
		);//ms vers ?
	}
}

void	ms_get_new_path_for_prompt(\
	t_ms *env, t_env_lst *envp_lst, t_prompt *cmd_prompt, bool first_time)
{
	t_env_lst	*pwd_node;
	t_env_lst	*home_node;
	size_t		key_len;
	char		*current_path;
	char		*home_path;
	size_t		new_path_len;

	pwd_node = ms_lst_get_node_with_the_same_key(envp_lst, "PWD=");
	key_len = 4;
	current_path = pwd_node->entry + key_len;
	home_node = ms_lst_get_node_with_the_same_key(envp_lst, "HOME=");
	if (home_node == NULL)
	{
		home_path = NULL;
		env->cmd_prompt.home_path_len = 0;
		new_path_len = MS_PMP_NO_HOME;
	}
	else
	{
		home_path = home_node->entry + key_len + 1;
		if (ms_strcmp(cmd_prompt->prompt, current_path) == MS_SAME)
			return ;
		new_path_len = ms_get_new_path_length(\
			home_path, current_path, cmd_prompt->home_path_len \
		);
	}
	ms_generate_new_path_for_prompt(env, current_path, new_path_len, false);
	ms_set_first_part_of_cmd_prompt(env, &env->cmd_prompt, first_time);
}
