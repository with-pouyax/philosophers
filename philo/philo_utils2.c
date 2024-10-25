/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/26 00:33:12 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_ate_enough(t_philosopher *philo, int value)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->ate_enough = value;
	pthread_mutex_unlock(&philo->philo_mutex);
}

int	get_ate_enough(t_philosopher *philo)
{
	int	value;

	pthread_mutex_lock(&philo->philo_mutex);
	value = philo->ate_enough;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (value);
}
