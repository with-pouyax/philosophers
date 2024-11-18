/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/18 18:22:16 by pghajard         ###   ########.fr       */
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
    if (philo->id % 2 == 0)
		usleep(100); // Even philosophers wait 100 microseconds
	else
		usleep(200);
    // Stagger start times by half of time_to_eat
   //usleep((philo->id - 1) * 1000);
}

// why do we need to add an initial delay, 1000 for even philosophers and 2000 for odd philosophers ? 
// be

void	*philosopher_life(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	pthread_mutex_lock(&philo->data->start_mutex); // Wait for the simulation to start
	pthread_mutex_unlock(&philo->data->start_mutex);
	initial_delay(philo); // Initial delay to prevent deadlocks
	set_last_meal_time(philo, get_time_in_ms());
	while (!get_simulation_end(philo->data))
	{
		philosopher_eat(philo);
		if (get_simulation_end(philo->data))
			break;
		print_message(philo, "is sleeping");
		custom_usleep(philo->data->time_to_sleep, philo->data);
		if (get_simulation_end(philo->data))
			break;
		print_message(philo, "is thinking");
	}
	return (NULL);
}
