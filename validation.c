/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:17:23 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 10:19:35 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	validate_other_arguments(int argc, char **argv)
{
	int			i;
	long long	arg_value;

	i = 2;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			print_error("Error: Invalid argument.\n", 25);
			return (0);
		}
		arg_value = ft_atoll(argv[i]);
		if (arg_value <= 0 || arg_value > LLONG_MAX)
		{
			print_error("Error: Argument out of valid range.\n", 35);
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_num_meals(const char *str)
{
	long long	num_meals;

	num_meals = ft_atoll(str);
	if (num_meals <= 0 || num_meals > INT_MAX)
	{
		print_error("Error: Number of meals must be a positive integer.\n", 47);
		return (0);
	}
	return (1);
}

int	validate_philosopher_count(const char *str)
{
	long long	philosophers;

	philosophers = ft_atoll(str);
	if (philosophers <= 0 || philosophers > BUFFER_SIZE)
	{
		print_error("Error: Number of philosophers must be \
						between 1 and 200.\n", 58);
		return (0);
	}
	return (1);
}

int	validate_arguments(int argc, char **argv)
{
	if (!validate_philosopher_count(argv[1]))
		return (0);
	if (!validate_other_arguments(argc, argv))
		return (0);
	if (argc == 6 && !validate_num_meals(argv[5]))
		return (0);
	return (1);
}
