/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:01:47 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 12:05:48 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		print_error("Error: Invalid number of arguments.\n", 36);
		return (0);
	}
	return (1);
}

void	print_error(const char *message, size_t len)
{
	write(2, message, len);
}

void	initialize_philosophers(t_data *data)
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

void	initialize_forks(t_data *data)
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

int	initialize_mutexes(t_data *data)
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
