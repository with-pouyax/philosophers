/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:50:17 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 14:46:16 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philosopher *philo, char *message)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->output_mutex); // lock the output mutex to prevent multiple threads from writing
	if (!get_simulation_end(philo->data)) // if the simulation has not ended yet
	{
		timestamp = get_time_in_ms() - philo->data->start_time; // get the current time in milliseconds
		printf("%lld %d %s\n", timestamp, philo->id, message); // print the timestamp, philosopher id, and message
	}
	pthread_mutex_unlock(&philo->data->output_mutex); // unlock the output mutex
}

void	join_philosopher_threads(t_data *data, pthread_t *philosophers, \
				int num_threads)
{
	int	i;

	(void)data;
	i = 0;
	while (i < num_threads) // loop through the number of threads
	{
		pthread_join(philosophers[i], NULL); // wait for the thread to in same order as they were created
		i++;
	}
}

int	create_philosopher_threads(t_data *data, pthread_t *philosophers)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers) // loop through the number of philosophers
	{
		set_last_meal_time(&data->philosophers[i], data->start_time); // set the last meal time of the philosopher to the start time
		if (pthread_create(&philosophers[i], NULL, philosopher_life,
				&data->philosophers[i]) != 0) // then create a philosopher thread
		{
			write(2, "Error: Failed to create philosopher thread.\n", 44);
			set_simulation_end(data, 1);
			return (i);
		}
		i++;
	}
	return (i);
}

int	check_philosopher_state(t_data *data, int i, int *all_ate_enough)
{
	long long	time_since_last_meal;

	time_since_last_meal = get_time_in_ms()
		- get_last_meal_time(&data->philosophers[i]); // get the time since the last meal
	if (time_since_last_meal >= data->time_to_die) // if the time since the last meal is greater than or equal to the time to die
	{
		print_message(&data->philosophers[i], "died"); // print that the philosopher has died
		set_simulation_end(data, 1); // set the simulation end to 1
		return (1); 
	}
	if (!get_ate_enough(&data->philosophers[i])) // if the philosopher has not eaten enough meals
		*all_ate_enough = 0; // set the all_ate_enough flag to 0
	return (0);
}

void	*monitor_philosophers(void *data_ptr)
{
	t_data	*data;
	int		all_ate_enough;
	int		i;

	data = (t_data *)data_ptr;
	while (!get_simulation_end(data)) // while the simulation has not ended
	{
		all_ate_enough = 1; // set the all_ate_enough flag to 1
		i = 0;
		while (i < data->num_philosophers) // loop through the philosophers
		{
			if (check_philosopher_state(data, i, &all_ate_enough)) // if someone died
				return (NULL);
			i++; // go to the next philosopher
		}
		if (data->num_meals != -1 && all_ate_enough) // if there is number of meals and all ate enough
		{
			set_simulation_end(data, 1); // set the simulation end to 1
			return (NULL); // return NULL
		}
		custom_usleep(1, data); // sleep for 1 millisecond
	}
	return (NULL);
}
