/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:15:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/26 00:57:16 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_simulation_end(t_data *data, int value)
{
	pthread_mutex_lock(&data->simulation_end_mutex);
	data->simulation_end = value;
	pthread_mutex_unlock(&data->simulation_end_mutex);
}

int	get_simulation_end(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->simulation_end_mutex);
	value = data->simulation_end;
	pthread_mutex_unlock(&data->simulation_end_mutex);
	return (value);
}
