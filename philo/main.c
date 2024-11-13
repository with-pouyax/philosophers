/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/13 15:08:41 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

static int	run_single_philosopher(t_data *data)
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

static int	start_simulation(t_data *data)
{
	pthread_t	philosophers[BUFFER_SIZE];
	pthread_t	monitor_thread;

	if (create_philosopher_threads(data, philosophers))
	{
		cleanup(data);
		return (1);
	}
	pthread_mutex_unlock(&data->start_mutex);
	if (pthread_create(&monitor_thread, NULL, monitor_philosophers, data) != 0)
	{
		write(2, "Error: Failed to create monitor thread.\n", 39);
		set_simulation_end(data, 1);
	}
	pthread_join(monitor_thread, NULL);
	join_philosopher_threads(data, philosophers);
	cleanup(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data)); // we initialize the data struct to 0
	if (!parse_arguments(argc, argv, &data)) // we parse the arguments
		return (1);
	if (initialize_simulation(&data)) // we initialize the simulation
		return (1);
	data.start_time = get_time_in_ms();
	pthread_mutex_lock(&data.start_mutex);
	if (data.num_philosophers == 1)
		return (run_single_philosopher(&data));
	else
		return (start_simulation(&data));
}
