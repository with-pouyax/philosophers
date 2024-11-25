/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:04:12 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 14:29:18 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_ate_enough(t_philosopher *philo, int value)
{
	pthread_mutex_lock(&philo->philo_mutex); // lock the philosopher mutex
	philo->ate_enough = value; // set the ate_enough value of the philosopher
	pthread_mutex_unlock(&philo->philo_mutex); // unlock the philosopher mutex
}

int	get_meals_eaten(t_philosopher *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->philo_mutex); // lock the philosopher mutex
	meals = philo->meals_eaten; // get the meals eaten by the philosopher
	pthread_mutex_unlock(&philo->philo_mutex); // unlock the philosopher mutex
	return (meals);
}

void	set_meals_eaten(t_philosopher *philo, int meals)
{
	pthread_mutex_lock(&philo->philo_mutex); // lock the philosopher mutex
	philo->meals_eaten = meals; // set the meals eaten by the philosopher
	pthread_mutex_unlock(&philo->philo_mutex); // unlock the philosopher mutex
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
