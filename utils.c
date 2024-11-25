/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:22:08 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 13:41:36 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_simulation_end(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->simulation_end_mutex); // lock the simulation_end_mutex to prevent multiple threads from reading
	value = data->simulation_end; // get the value of simulation_end
	pthread_mutex_unlock(&data->simulation_end_mutex); // unlock the simulation_end_mutex
	return (value);
}

void	set_simulation_end(t_data *data, int value)
{
	pthread_mutex_lock(&data->simulation_end_mutex); // lock the simulation_end_mutex
	data->simulation_end = value; // set the value of simulation_end
	pthread_mutex_unlock(&data->simulation_end_mutex); // unlock the simulation_end_mutex
}

void	custom_usleep(long long time_in_ms, t_data *data) // 1- we use it so we check if simulation has ended, even during the sleep
														  // 2- usleep my slighly oversleep, but now we check every 1 millisecond if we slept enough
{
	long long	start_time;

	start_time = get_time_in_ms(); // get the current time in milliseconds
	while (!get_simulation_end(data) 
		&& (get_time_in_ms() - start_time) < time_in_ms) // we break loop if simulation has ended
														 // also in each loop we check if the time we received has passed, and break if so
														 // we repeat this every 1000 microseconds, which is 1 millisecond
		usleep(1000);
}

long long	get_time_in_ms(void)
{
	struct timeval	tv; // struct timeval is a structure that contains two members: tv_sec and tv_usec

	gettimeofday(&tv, NULL); // gettimeofday() is a function that gets the current time and stores it in the tv structure 
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)); // tv.tv_sec * 1000LL converts the seconds to milliseconds.
	                                                     // LL is a suffix that tells the compiler to treat the number as a long long.
														 // tv.tv_usec / 1000 converts the microseconds to milliseconds.
														 // The sum of the two is the total time in milliseconds. 
}
