/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:00:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/13 15:08:24 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_error(const char *message, size_t len)
{
	write(2, message, len);
}

/**
 * @brief Checks if the number of arguments is valid.
 *
 * @param argc Argument count.
 * @return int Returns 1 if valid, 0 otherwise.
 */
static int	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		print_error("Error: Invalid number of arguments.\n", 36);
		return (0);
	}
	return (1);
}

/**
 * @brief Validates each argument to ensure they are valid numbers within range.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 * @return int Returns 1 if all arguments are valid, 0 otherwise.
 */
static int	validate_arguments(int argc, char **argv)
{
	int			i;
	long long	arg_value;

	i = 1;
	while (i < argc) // we iterate through the arguments
	{
		if (!is_valid_number(argv[i])) // we check if the argument is a valid number
		{
			print_error("Error: Invalid argument.\n", 25);
			return (0);
		}
		arg_value = ft_atoll(argv[i]); // we convert the argument to a long long
		if (arg_value <= 0 || arg_value > 4294967295) // we check if the argument is within range, 4294967295 is the maximum value for an unsigned int
		{
			print_error("Error: Argument out of valid range.\n", 35);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Parses and assigns command-line arguments to the data structure.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 * @param data Pointer to the data structure.
 * @return int Returns 1 on success, 0 on failure.
 */
int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (!check_argc(argc)) // we check if the number of arguments is valid
		return (0);
	if (!validate_arguments(argc, argv)) // we validate the arguments
		return (0); 
	data->num_philosophers = atoi(argv[1]); // we assign the number of philosophers
	data->time_to_die = ft_atoll(argv[2]); // we assign the time to die
	data->time_to_eat = ft_atoll(argv[3]); // we assign the time to eat
	data->time_to_sleep = ft_atoll(argv[4]); // we assign the time to sleep
	if (argc == 6) // if there is a 6th argument
		data->num_meals = atoi(argv[5]); // we assign the number of meals
	else // if there is no 6th argument
		data->num_meals = -1; // we assign -1 to the number of meals
	return (1);
}

static int	initialize_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->output_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->output_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->simulation_end_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->output_mutex);
		pthread_mutex_destroy(&data->start_mutex);
		return (1);
	}
	return (0);
}

static void	initialize_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->forks[i].id = i + 1;
		pthread_mutex_init(&data->forks[i].mutex, NULL);
		i++;
	}
}

static void	initialize_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].ate_enough = 0;
		data->philosophers[i].last_meal_time = 0;
		data->philosophers[i].right_fork = &data->forks[i];
		data->philosophers[i].left_fork = &data->forks[(i + 1)
			% data->num_philosophers];
		data->philosophers[i].data = data;
		pthread_mutex_init(&data->philosophers[i].philo_mutex, NULL);
		i++;
	}
}

int	initialize_simulation(t_data *data)
{
	if (initialize_mutexes(data))
	{
		write(2, "Error: Failed to initialize mutexes.\n", 36);
		return (1);
	}
	initialize_forks(data);
	initialize_philosophers(data);
	return (0);
}
