/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:47:48 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 12:08:35 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	parse_initial(const char *str, int *i, int *sign)
{
	*i = 0;
	*sign = 1;
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
	{
		(*i)++;
	}
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
		{
			*sign = -1;
		}
		(*i)++;
	}
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	parse_initial(str, &i, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LLONG_MAX / 10) || (result == (LLONG_MAX / 10) \
						&& (str[i] - '0') > (LLONG_MAX % 10)))
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
