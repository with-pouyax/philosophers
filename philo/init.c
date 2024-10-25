/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/26 00:54:53 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_arguments(int argc, char **argv, t_data *data)
{
	long long	arg_value;
	int			i;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error: Invalid number of arguments.\n", 36);
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			write(2, "Error: Invalid argument.\n", 25);
			return (0);
		}
		arg_value = ft_atoll(argv[i]);
		if (arg_value <= 0 || arg_value > 4294967295)
		{
			write(2, "Error: Argument out of valid range.\n", 35);
			return (0);
		}
		i++;
	}
	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	data->num_meals = (argc == 6) ? atoi(argv[5]) : -1;
	return (1);
}

static int	initialize_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->output_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->output_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->simulation_end_mutex, NULL) != 0)
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
	if (initialize_mutexes(data))
	{
		write(2, "Error: Failed to initialize mutexes.\n", 36);
		return (1);
	}
	initialize_forks(data);
	initialize_philosophers(data);
	return (0);
}
