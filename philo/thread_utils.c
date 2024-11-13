/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/13 15:25:00 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philosopher_threads(t_data *data, pthread_t *philosophers)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers) // we iterate through the number of philosophers and create a thread for each one
	{
		set_last_meal_time(&data->philosophers[i], data->start_time); // we set the last meal time for the philosopher to the start time
		if (pthread_create(&philosophers[i], NULL, philosopher_life,
				&data->philosophers[i]) != 0) // we create the philosopher thread
		{
			write(2, "Error: Failed to create philosopher thread.\n", 44);
			set_simulation_end(data, 1);
			return (1);
		}
		i++; // we go to the next philosopher
	}
	return (0);
}

void	join_philosopher_threads(t_data *data, pthread_t *philosophers)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_join(philosophers[i], NULL);
		i++;
	}
}
