/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:02:51 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 12:06:20 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_ate_enough(t_philosopher *philo)
{
	int	value;

	pthread_mutex_lock(&philo->philo_mutex);
	value = philo->ate_enough;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (value);
}
