/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strings_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 16:29:43 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/16 22:41:23 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_reset_color_settings(void)
{
	write(STDOUT_FILENO, "\x1B[0m", 4);
}

char	*ms_color_string(t_ms *env, char *str, char *color)
{
	char	*colored_str;

	colored_str = ppx_join_three_str(\
		&env->ppx_env, \
		color, \
		str, \
		MS_CLR_FAINT_WHITE);
	return (colored_str);
}

void	ms_print_if_not_backslash(char *arg)
{
	while (*arg)
	{
		if (*arg != '\\')
			write(STDOUT_FILENO, &*arg, 1);
		arg++;
	}
}

void	ppx_free_array_of_pointers(char ***array_of_pointers, size_t arr_size)
{
	size_t	i;

	if (*array_of_pointers == NULL)
		return ;
	i = 0;
	if (arr_size != 0)
	{
		while (i < arr_size)
		{
			(*array_of_pointers)[i] = ms_free((*array_of_pointers)[i]);
			++i;
		}
	}
	else
	{
		while ((*array_of_pointers)[i])
		{
			(*array_of_pointers)[i] = ms_free((*array_of_pointers)[i]);
			++i;
		}
	}
	*array_of_pointers = ms_free(*array_of_pointers);
}

char	*ppx_join_three_str(t_ppx *env, char *str1, char *str2, char *str3)
{
	char	*joined;
	size_t	joined_len;
	size_t	str1_len;
	size_t	str2_len;
	size_t	str3_len;

	if (!str1 || !str2 || !str3)
		ppx_exit_with_error_message(env, 2);
	str1_len = ms_strlen(str1);
	str2_len = ms_strlen(str2);
	str3_len = ms_strlen(str3);
	joined_len = str1_len + str2_len + str3_len;
	joined = ppx_malloc(env, joined_len + 1, sizeof(*joined));
	ppx_memcpy(joined, str1, str1_len);
	ppx_memcpy(&joined[str1_len], str2, str2_len);
	ppx_memcpy(&joined[str1_len + str2_len], str3, str3_len);
	joined[joined_len] = '\0';
	return (joined);
}
