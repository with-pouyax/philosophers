/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:17:23 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 11:11:51 by pghajard         ###   ########.fr       */
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
		if (!is_valid_number(argv[i])) // if the argument contains a non-numeric character
		{
			print_error("Error: Invalid argument.\n", 25);
			return (0);
		}
		arg_value = ft_atoll(argv[i]); // convert the argument to a long long
		if (arg_value <= 0 || arg_value > LLONG_MAX) // if the argument is less than or equal to 0 or greater than the maximum long long value
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

	if (!is_valid_number(str)) // if the argument contains a non-numeric character
		{
			print_error("Error: Invalid argument.\n", 25);
			return (0);
		}
	philosophers = ft_atoll(str); // convert the number of philosophers to a long long
	if (philosophers <= 0 || philosophers > BUFFER_SIZE) 
	{
		print_error("philosophers must be between 1 and 200.\n", 40);
		return (0);
	}
	return (1);
}

int	validate_arguments(int argc, char **argv)
{
	if (!validate_philosopher_count(argv[1])) // validate the number of philosophers
		return (0);
	if (!validate_other_arguments(argc, argv)) // validate the other arguments
		return (0);
	if (argc == 6 && !validate_num_meals(argv[5])) // if the number of arguments is 6, validate the number of meals
		return (0);
	return (1);
}