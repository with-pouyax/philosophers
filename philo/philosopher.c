/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/26 00:54:02 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philosopher *philo)
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
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	set_meals_eaten(philo, get_meals_eaten(philo) + 1);
	if (philo->data->num_meals != -1
		&& get_meals_eaten(philo) >= philo->data->num_meals)
		set_ate_enough(philo, 1);
}

static void	initial_delay(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
		usleep(1000 * philo->data->time_to_eat);
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
		print_message(philo, "is sleeping");
		custom_usleep(philo->data->time_to_sleep, philo->data);
		print_message(philo, "is thinking");
	}
	return (NULL);
}
