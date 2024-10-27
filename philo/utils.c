/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:15:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/27 16:12:16 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;
	int			overflow;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)) // skip whitespace characters
		i++;
	if (str[i] == '-' || str[i] == '+') // check if the number is negative
	{
		if (str[i++] == '-') // if the number is negative, set the sign to -1
			sign = -1; 
	}
	while (str[i] >= '0' && str[i] <= '9') // iterate through the string
	{
		overflow = check_overflow(result, sign, str[i]); // check if adding the next digit will cause an overflow
		if (overflow != 1)
			return (overflow);
		result = result * 10 + (str[i++] - '0');
	}
	return (result * sign);
}

int	check_overflow(long long result, int sign, char c)
{
	if (result > LLONG_MAX / 10) // if result is greater than LLONG_MAX / 10, then adding the next digit will cause an overflow
	{
		if (sign == 1) // if the number is positive, return -1
			return (-1);
		else
			return (0); // if the number is negative, return 0
	}
	if (result == LLONG_MAX / 10 &&
		(c - '0') > (LLONG_MAX % 10)) /* if LLONG_MAX / 10 is exactly equal to result then adding the next digit
		will cause an overflow only if the next digit is greater than LLONG_MAX % 10 because LLONG_MAX % 10 is the
		last digit of LLONG_MAX */
	{
		if (sign == 1)
			return (-1);
		else
			return (0);
	}
	return (1);
}

int	is_valid_number(const char *str) // check if the string is a valid number
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-') // if the first character is a + or a -, skip it
		i++;
	if (str[i] == '\0') // if the first character is the null terminator, return 0
		return (0);
	while (str[i]) // iterate through the string
	{
		if (str[i] < '0' || str[i] > '9') // if the character is not a number, return 0
			return (0);
		i++; // otherwise, go to the next character
	}
	return (1); // if all characters are numbers, return 1
}

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	custom_usleep(long long time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_time_in_ms();
	while (!get_simulation_end(data) &&
		(get_time_in_ms() - start_time) < time_in_ms)
		usleep(100);
}
