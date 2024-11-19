/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:48:46 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 09:49:47 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_data *data)
{
	pthread_t	philosophers[BUFFER_SIZE];
	pthread_t	monitor_thread;
	int			created_threads;

	created_threads = create_philosopher_threads(data, philosophers);
	if (created_threads != data->num_philosophers)
	{
		pthread_mutex_unlock(&data->start_mutex);
		join_philosopher_threads(data, philosophers, created_threads);
		cleanup(data);
		return (1);
	}
	pthread_mutex_unlock(&data->start_mutex);
	if (pthread_create(&monitor_thread, NULL, monitor_philosophers, data) != 0)
	{
		write(2, "Error: Failed to create monitor thread.\n", 39);
		set_simulation_end(data, 1);
		join_philosopher_threads(data, philosophers, data->num_philosophers);
		cleanup(data);
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	join_philosopher_threads(data, philosophers, data->num_philosophers);
	cleanup(data);
	return (0);
}

int	run_single_philosopher(t_data *data)
{
	pthread_t	philosopher;

	pthread_mutex_unlock(&data->start_mutex);
	if (pthread_create(&philosopher, NULL, single_philosopher,
			&data->philosophers[0]) != 0)
	{
		write(2, "Error: Failed to create philosopher thread.\n", 44);
		cleanup(data);
		return (1);
	}
	pthread_join(philosopher, NULL);
	cleanup(data);
	return (0);
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

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (!parse_arguments(argc, argv, &data))
		return (1);
	if (initialize_simulation(&data))
		return (1);
	data.start_time = get_time_in_ms();
	pthread_mutex_lock(&data.start_mutex);
	if (data.num_philosophers == 1)
		return (run_single_philosopher(&data));
	else
		return (start_simulation(&data));
}
