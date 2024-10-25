/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/26 00:33:25 by pouyaximac       ###   ########.fr       */
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
