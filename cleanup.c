/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:47:51 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 09:48:02 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		pthread_mutex_destroy(&data->philosophers[i].philo_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->output_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	pthread_mutex_destroy(&data->simulation_end_mutex);
}
