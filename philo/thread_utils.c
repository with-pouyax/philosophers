/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/18 23:17:00 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philosopher_threads(t_data *data, pthread_t *philosophers)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		set_last_meal_time(&data->philosophers[i], data->start_time);
		if (pthread_create(&philosophers[i], NULL, philosopher_life,
				&data->philosophers[i]) != 0)
		{
			write(2, "Error: Failed to create philosopher thread.\n", 44);
			set_simulation_end(data, 1);
			return (i);
		}
		i++;
	}
	return (i);
}

void	join_philosopher_threads(t_data *data, pthread_t *philosophers, int num_threads)
{
	int	i;
	(void)data;

	i = 0;
	while (i < num_threads)
	{
		pthread_join(philosophers[i], NULL);
		i++;
	}
}