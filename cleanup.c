/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:47:51 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 13:43:53 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers) // loop through the philosophers
	{
		pthread_mutex_destroy(&data->forks[i].mutex); // destroy the mutex of the fork
		pthread_mutex_destroy(&data->philosophers[i].philo_mutex); // destroy the mutex of the philosopher
		i++;
	}
	pthread_mutex_destroy(&data->output_mutex); // destroy the output mutex
	pthread_mutex_destroy(&data->start_mutex); // destroy the start mutex
	pthread_mutex_destroy(&data->simulation_end_mutex); // destroy the simulation end mutex
}
