/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/16 23:50:49 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_state(t_data *data, int i, int *all_ate_enough)
{
	long long	time_since_last_meal;

	time_since_last_meal = get_time_in_ms()
		- get_last_meal_time(&data->philosophers[i]);
	if (time_since_last_meal >= data->time_to_die)
	{
		print_message(&data->philosophers[i], "died");
		set_simulation_end(data, 1);
		return (1);
	}
	if (!get_ate_enough(&data->philosophers[i]))
		*all_ate_enough = 0;
	return (0);
}

void	*monitor_philosophers(void *data_ptr)
{
	t_data	*data;
	int		all_ate_enough;
	int		i;

	data = (t_data *)data_ptr;
	while (!get_simulation_end(data))
	{
		all_ate_enough = 1;
		i = 0;
		while (i < data->num_philosophers)
		{
			if (check_philosopher_state(data, i, &all_ate_enough))
				return (NULL);
			i++;
		}
		if (data->num_meals != -1 && all_ate_enough)
		{
			set_simulation_end(data, 1);
			return (NULL);
		}
		custom_usleep(1, data);
	}
	return (NULL);
}
