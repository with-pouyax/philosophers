/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:55:12 by pghajard          #+#    #+#             */
/*   Updated: 2024/11/25 14:35:39 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_philosopher(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher; 
	pthread_mutex_lock(&philo->data->start_mutex); // here if start_mutex is still locked from the main, it will wait until it is unlocked
	pthread_mutex_unlock(&philo->data->start_mutex); //it will be unlocked in the run_single_philosopher function 
	print_message(philo, "has taken a fork"); // print that the philosopher has taken a fork
	custom_usleep(philo->data->time_to_die, philo->data); // sleep for the time_to_die
	print_message(philo, "died"); // print that the philosopher has died
	set_simulation_end(philo->data, 1); // set the simulation end to 1
	return (NULL);
}

void ft_usleep(long long time_in_ms)
{
	long long start_time;

	start_time = get_time_in_ms();
	while ((get_time_in_ms() - start_time) < time_in_ms)
		usleep(10);
}

void	initial_delay(t_philosopher *philo)
{
	if (philo->id % 2 == 0) // if the philosopher id is even
		ft_usleep(100);  
	else // if the philosopher id is odd
		ft_usleep(200); // sleep for 200 microseconds
}

void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0) // if the philosopher id is even
	{
		pthread_mutex_lock(&philo->left_fork->mutex); // lock the left fork mutex
		print_message(philo, "has taken a fork"); // print that the philosopher has taken a fork
		pthread_mutex_lock(&philo->right_fork->mutex); // lock the right fork mutex
		print_message(philo, "has taken a fork"); // print that the philosopher has taken a fork
	}
	else // if the philosopher id is odd
	{
		custom_usleep(1, philo->data); // sleep for 1 millisecond
		pthread_mutex_lock(&philo->right_fork->mutex); // lock the right fork mutex
		print_message(philo, "has taken a fork"); // print that the philosopher has taken a fork
		pthread_mutex_lock(&philo->left_fork->mutex); // lock the left fork mutex
		print_message(philo, "has taken a fork"); // print that the philosopher has taken a fork
	}
}

void	philosopher_eat(t_philosopher *philo)
{
	if (get_simulation_end(philo->data))
		return ;
	take_forks(philo); // the philosopher takes the forks
	set_last_meal_time(philo, get_time_in_ms()); // set the last meal time of the philosopher to the current time
	print_message(philo, "is eating");
	custom_usleep(philo->data->time_to_eat, philo->data); // sleep for the time to eat
	pthread_mutex_unlock(&philo->right_fork->mutex); // unlock the right fork mutex
	pthread_mutex_unlock(&philo->left_fork->mutex); // unlock the left fork mutex
	set_meals_eaten(philo, get_meals_eaten(philo) + 1); // increment the number of meals eaten by the philosopher
	if (philo->data->num_meals != -1
		&& get_meals_eaten(philo) >= philo->data->num_meals) // if the number of meals is not -1
															 // and the number of meals eaten by the philosopher is greater than or equal
															 // to the number of meals
		set_ate_enough(philo, 1); // set the ate_enough flag to 1
}

void	*philosopher_life(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	pthread_mutex_lock(&philo->data->start_mutex); // lock the start mutex to see if it is already unlocked or not
	pthread_mutex_unlock(&philo->data->start_mutex); // unlock the start mutex
	initial_delay(philo); // sleep for a short time
	set_last_meal_time(philo, get_time_in_ms()); // set the last meal time of the philosopher to the current time
	while (!get_simulation_end(philo->data)) // loop until the simulation ends
	{
		philosopher_eat(philo); // the philosopher eats
		if (get_simulation_end(philo->data)) // if the simulation ends during eating break the loop
			break ;
		print_message(philo, "is sleeping"); // print that the philosopher is sleeping
		custom_usleep(philo->data->time_to_sleep, philo->data); // sleep for the time to sleep
		if (get_simulation_end(philo->data)) // if the simulation ends during sleeping break the loop
			break ;
		print_message(philo, "is thinking"); // print that the philosopher is thinking
	}
	return (NULL);
}
