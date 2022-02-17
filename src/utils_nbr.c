/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_nbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 05:13:39 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/17 01:31:44 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_get_absolute_value(long long int nbr)
{
	if (nbr < 0)
		return (-nbr);
	return (nbr);
}

size_t	ms_nbrlen(long long int n)
{
	size_t	len;

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
	nb = ms_get_absolute_value(nb);
	while (len--)
	{
		res[len] = nb % 10 + '0';
		nb /= 10;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
}
