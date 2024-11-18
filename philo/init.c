/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/18 14:55:30 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_error(const char *message, size_t len)
{
	write(2, message, len);
}


static int	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		print_error("Error: Invalid number of arguments.\n", 36);
		return (0);
	}
	return (1);
}
static int	validate_philosopher_count(const char *str)
{
	long long	philosophers;

	philosophers = ft_atoll(str);
	if (philosophers <= 0 || philosophers > BUFFER_SIZE)
	{
		print_error("Error: Number of philosophers must be between 1 and 200.\n", 58);
		return (0);
	}
	return (1);
}

static int	validate_num_meals(const char *str)
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

static int	validate_other_arguments(int argc, char **argv)
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

static int	validate_arguments(int argc, char **argv)
{
	if (!validate_philosopher_count(argv[1]))
		return (0);
	if (!validate_other_arguments(argc, argv))
		return (0);
	if (argc == 6 && !validate_num_meals(argv[5]))
		return (0);
	return (1);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	long long	temp;

	if (!check_argc(argc))
		return (0);
	if (!validate_arguments(argc, argv))
		return (0);
	temp = ft_atoll(argv[1]);
	data->num_philosophers = (int)temp;
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
	{
		temp = ft_atoll(argv[5]);
		data->num_meals = (int)temp;
	}
	else
		data->num_meals = -1;
	return (1);
}

static int	initialize_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->output_mutex, NULL) != 0) // we initialize the output mutex
		return (1);
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0) // we initialize the start mutex
	{
		pthread_mutex_destroy(&data->output_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->simulation_end_mutex, NULL) != 0) // we initialize the simulation end mutex
	{
		pthread_mutex_destroy(&data->output_mutex);
		pthread_mutex_destroy(&data->start_mutex);
		return (1);
	}
	return (0);
}

static void	initialize_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->forks[i].id = i + 1;
		pthread_mutex_init(&data->forks[i].mutex, NULL);
		i++;
	}
}

static void	initialize_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].ate_enough = 0;
		data->philosophers[i].last_meal_time = 0;
		data->philosophers[i].right_fork = &data->forks[i];
		data->philosophers[i].left_fork = &data->forks[(i + 1)
			% data->num_philosophers];
		data->philosophers[i].data = data;
		pthread_mutex_init(&data->philosophers[i].philo_mutex, NULL);
		i++;
	}
}

int	initialize_simulation(t_data *data)
{
	if (initialize_mutexes(data)) // we initialize the mutexes
	{
		write(2, "Error: Failed to initialize mutexes.\n", 36);
		return (1);
	}
	initialize_forks(data);
	initialize_philosophers(data);
	return (0);
}
