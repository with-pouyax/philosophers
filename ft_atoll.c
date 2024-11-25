/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:47:48 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 10:25:31 by pghajard         ###   ########.fr       */
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
	parse_initial(str, &i, &sign); // skip any leading whitespace and check for a sign
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LLONG_MAX / 10) || (result == (LLONG_MAX / 10) \
						&& (str[i] - '0') > (LLONG_MAX % 10))) // LLONG_MAX / 10, because later we multiply result by 10
															   // result == LLONG_MAX / 10 because if result is equal to LLONG_MAX / 10 
															   // and the next digit is greater than LLONG_MAX % 10, which is 7, then the result will overflow
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
