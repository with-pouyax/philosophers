/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:55:12 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/19 09:57:33 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_philosopher(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	print_message(philo, "has taken a fork");
	custom_usleep(philo->data->time_to_die, philo->data);
	print_message(philo, "died");
	set_simulation_end(philo->data, 1);
	return (NULL);
}

void	initial_delay(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
		usleep(100);
	else
		usleep(200);
}

void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_message(philo, "has taken a fork");
	}
	else
	{
		custom_usleep(1, philo->data);
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_message(philo, "has taken a fork");
	}
}

void	philosopher_eat(t_philosopher *philo)
{
	if (get_simulation_end(philo->data))
		return ;
	take_forks(philo);
	set_last_meal_time(philo, get_time_in_ms());
	print_message(philo, "is eating");
	custom_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	set_meals_eaten(philo, get_meals_eaten(philo) + 1);
	if (philo->data->num_meals != -1
		&& get_meals_eaten(philo) >= philo->data->num_meals)
		set_ate_enough(philo, 1);
}

void	*philosopher_life(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	initial_delay(philo);
	set_last_meal_time(philo, get_time_in_ms());
	while (!get_simulation_end(philo->data))
	{
		philosopher_eat(philo);
		if (get_simulation_end(philo->data))
			break ;
		print_message(philo, "is sleeping");
		custom_usleep(philo->data->time_to_sleep, philo->data);
		if (get_simulation_end(philo->data))
			break ;
		print_message(philo, "is thinking");
	}
	return (NULL);
}
