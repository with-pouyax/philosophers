/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/26 00:32:57 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_last_meal_time(t_philosopher *philo, long long time)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->philo_mutex);
}

long long	get_last_meal_time(t_philosopher *philo)
{
	long long	time;

	pthread_mutex_lock(&philo->philo_mutex);
	time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (time);
}

void	set_meals_eaten(t_philosopher *philo, int meals)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->meals_eaten = meals;
	pthread_mutex_unlock(&philo->philo_mutex);
}

int	get_meals_eaten(t_philosopher *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->philo_mutex);
	meals = philo->meals_eaten;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (meals);
}
