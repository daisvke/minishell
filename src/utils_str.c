/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 16:29:43 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/21 12:05:54 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ppx_exit_with_error_message(env, 8);
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

int	ms_nbrlen(long long int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
	{
		n = -n;
		len = ms_nbrlen(n) + 1;
	}
	else
	{
		while (n > 0)
		{
			len += 1;
			n = n / 10;
		}
	}
	return (len);
}

char	*ms_itoa(t_ms *env, int n)
{
	char	*res;
	int		len;
	long	nb;

	nb = n;
	len = ms_nbrlen(nb);
	res = (char *)malloc(sizeof(*res) * (len + 1));
	if (!res)
		ms_exit_with_error_message(env, 16);
	res[len] = '\0';
	if (nb < 0)
		nb = -nb;
	while (len--)
	{
		res[len] = nb % 10 + '0';
		nb /= 10;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
}
