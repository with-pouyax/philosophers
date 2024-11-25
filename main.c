/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:48:46 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 14:45:28 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_data *data)
{
	pthread_t	philosophers[BUFFER_SIZE];
	pthread_t	monitor_thread;
	int			created_threads;

	created_threads = create_philosopher_threads(data, philosophers); // create the philosopher threads 
	if (created_threads != data->num_philosophers)
	{
		pthread_mutex_unlock(&data->start_mutex);
		join_philosopher_threads(data, philosophers, created_threads);
		cleanup(data);
		return (1);
	}
	pthread_mutex_unlock(&data->start_mutex); // unlock the start mutex once all the threads are created
	if (pthread_create(&monitor_thread, NULL, monitor_philosophers, data) != 0) // create the monitor thread
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

	pthread_mutex_unlock(&data->start_mutex); // unlock the start mutex
	if (pthread_create(&philosopher, NULL, single_philosopher,
			&data->philosophers[0]) != 0) // create a single philosopher thread
	{
		write(2, "Error: Failed to create philosopher thread.\n", 44);
		cleanup(data);
		return (1);
	}
	pthread_join(philosopher, NULL); // wait for the philosopher thread to finish
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

	if (!check_argc(argc)) // check the number of arguments
		return (0);
	if (!validate_arguments(argc, argv)) // validate the arguments
		return (0);
	temp = ft_atoll(argv[1]); // convert the first argument to a long long
	data->num_philosophers = (int)temp; // set the number of philosophers
	data->time_to_die = ft_atoll(argv[2]); // set the time to die
	data->time_to_eat = ft_atoll(argv[3]); // set the time to eat
	data->time_to_sleep = ft_atoll(argv[4]); // set the time to sleep
	if (argc == 6) // if the number of arguments is 6
	{
		temp = ft_atoll(argv[5]); 
		data->num_meals = (int)temp; // set the number of meals
	}
	else
		data->num_meals = -1; // set the number of meals to -1
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data)); // memset is usset to initialize the data struct to 0
	if (!parse_arguments(argc, argv, &data)) // parse the arguments
		return (1);
	if (initialize_simulation(&data)) // initialize the simulation
		return (1);
	data.start_time = get_time_in_ms(); // get the current time in milliseconds
	pthread_mutex_lock(&data.start_mutex); // lock the start mutex to prevent the threads from starting before the simulation is initialized
	if (data.num_philosophers == 1)
		return (run_single_philosopher(&data));
	else // if there is more than one philosopher
		return (start_simulation(&data));
}
