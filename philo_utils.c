/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:04:12 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 10:08:04 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_ate_enough(t_philosopher *philo, int value)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->ate_enough = value;
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

void	set_meals_eaten(t_philosopher *philo, int meals)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->meals_eaten = meals;
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

void	set_last_meal_time(t_philosopher *philo, long long time)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->philo_mutex);
}
