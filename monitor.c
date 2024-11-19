/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:50:17 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 09:52:04 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philosopher *philo, char *message)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->output_mutex);
	if (!get_simulation_end(philo->data))
	{
		timestamp = get_time_in_ms() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, message);
	}
	pthread_mutex_unlock(&philo->data->output_mutex);
}

void	join_philosopher_threads(t_data *data, pthread_t *philosophers, \
				int num_threads)
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

int	check_philosopher_state(t_data *data, int i, int *all_ate_enough)
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
