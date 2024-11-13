/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/13 15:28:39 by pghajard         ###   ########.fr       */
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
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
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

static void	initial_delay(t_philosopher *philo)
{
	if (philo->data->num_philosophers % 2 == 0)
	{
		if (philo->id % 2 == 0)
			usleep(1000);
	}
	else
	{
		if (philo->id % 3 == 0)
			usleep(2000);
		else if (philo->id % 3 == 1)
			usleep(4000);
	}
}

void	*philosopher_life(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	pthread_mutex_lock(&philo->data->start_mutex); // we lock the start mutex to wait for the threads to be created
	pthread_mutex_unlock(&philo->data->start_mutex); // we unlock the start mutex to wait for the threads to be created
	initial_delay(philo); // we add an initial delay to avoid deadlocks
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
