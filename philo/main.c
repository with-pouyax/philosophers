/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/18 18:38:57 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

static int	run_single_philosopher(t_data *data)
{
	pthread_t	philosopher;

	pthread_mutex_unlock(&data->start_mutex); // we unlock the start mutex
	if (pthread_create(&philosopher, NULL, single_philosopher,
			&data->philosophers[0]) != 0) // we create the philosopher thread
	{
		write(2, "Error: Failed to create philosopher thread.\n", 44);
		cleanup(data);
		return (1);
	}
	pthread_join(philosopher, NULL); // we wait for the philosopher thread to finish
	cleanup(data); // we cleanup the data
	return (0);
}

static int	start_simulation(t_data *data)
{
	pthread_t	philosophers[BUFFER_SIZE];
	pthread_t	monitor_thread;
	int			created_threads;

	created_threads = create_philosopher_threads(data, philosophers);
	if (created_threads != data->num_philosophers)
	{
		pthread_mutex_unlock(&data->start_mutex); // Allow philosopher threads to proceed
		join_philosopher_threads(data, philosophers, created_threads); // Join only created threads
		cleanup(data);
		return (1);
	}
	pthread_mutex_unlock(&data->start_mutex); // Unlock to start the simulation
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

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data)); // Initialize the data struct to 0
	if (!parse_arguments(argc, argv, &data)) // Parse the arguments
		return (1);
	if (initialize_simulation(&data)) // Initialize the simulation
		return (1);
	data.start_time = get_time_in_ms(); // Get the current time in ms
	pthread_mutex_lock(&data.start_mutex); // Lock the start mutex to wait for the threads to be created
	if (data.num_philosophers == 1) // If there is only one philosopher
		return (run_single_philosopher(&data)); // Run the single philosopher simulation
	else
		return (start_simulation(&data)); // Start the simulation with multiple philosophers
}
