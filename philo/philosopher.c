/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/16 20:09:52 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0) // Even philosophers
	{
		pthread_mutex_lock(&philo->left_fork->mutex); // Lock left fork
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex); // Lock right fork
		print_message(philo, "has taken a fork");
	}
	else // Odd philosophers
	{
		custom_usleep(1, philo->data); // We add a delay to avoid deadlocks
		pthread_mutex_lock(&philo->right_fork->mutex); // Lock right fork first
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex); // Then lock left fork
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
			usleep(10);
	}
	else
	{
		if (philo->id % 3 == 0)
			usleep(20);
		else if (philo->id % 3 == 1)
			usleep(40);
	}
    // Stagger start times by half of time_to_eat
   //usleep((philo->id - 1) * 1000);
}

// why do we need to add an initial delay, 1000 for even philosophers and 2000 for odd philosophers ? 
// be

void	*philosopher_life(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	pthread_mutex_lock(&philo->data->start_mutex); // we lock the start mutex to wait for the threads to be created
	pthread_mutex_unlock(&philo->data->start_mutex); // we unlock the start mutex to wait for the threads to be created
	initial_delay(philo); // we add an initial delay, this will make a delay for 
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
