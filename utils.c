/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:22:08 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 15:46:48 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_simulation_end(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->simulation_end_mutex);
	value = data->simulation_end;
	pthread_mutex_unlock(&data->simulation_end_mutex);
	return (value);
}

void	set_simulation_end(t_data *data, int value)
{
	pthread_mutex_lock(&data->simulation_end_mutex);
	data->simulation_end = value;
	pthread_mutex_unlock(&data->simulation_end_mutex);
}

void	custom_usleep(long long time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_time_in_ms();
	while (!get_simulation_end(data)
		&& (get_time_in_ms() - start_time) < time_in_ms)
		usleep(100);
}

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}
