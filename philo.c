#include "philo.h"

long long	get_time_in_ms(t_data *data)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - data->start_time;
}

void	precise_usleep(long long time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_time_in_ms(data);
	while (get_time_in_ms(data) - start_time < time_in_ms)
	{
		if (data->simulation_end)
			break ;
		usleep(100);
	}
}

void	print_message(t_philosopher *philo, char *message)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->simulation_end)
		printf("%lld %d %s\n", get_time_in_ms(philo->data), philo->id, message);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	philosopher_eat(t_philosopher *philo)
{
	if (philo->data->simulation_end)
		return ;
	if (philo->data->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_message(philo, "has taken a fork");
		precise_usleep(philo->data->time_to_die, philo->data);
		print_message(philo, "died");
		pthread_mutex_unlock(&philo->left_fork);
		return ;
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_message(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork");
	}
	print_message(philo, "is eating");
	philo->last_meal_time = get_time_in_ms(philo->data);
	precise_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	philo->meals_eaten++;
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->number_of_meals != -1 && philo->meals_eaten >= philo->data->number_of_meals)
	{
		int	all_ate_enough = 1;
		for (int i = 0; i < philo->data->number_of_philosophers; i++)
		{
			if (philo->data->philosophers[i].meals_eaten < philo->data->number_of_meals)
			{
				all_ate_enough = 0;
				break ;
			}
		}
		if (all_ate_enough)
			philo->data->simulation_end = 1;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
}

void	*philosopher_life(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;
	while (!philo->data->simulation_end)
	{
		philosopher_eat(philo);
		if (philo->data->simulation_end)
			break ;
		print_message(philo, "is sleeping");
		precise_usleep(philo->data->time_to_sleep, philo->data);
		if (philo->data->simulation_end)
			break ;
		print_message(philo, "is thinking");
	}
	return (NULL);
}

void	*death_monitor(void *data_void)
{
	t_data	*data;

	data = (t_data *)data_void;
	while (!data->simulation_end)
	{
		for (int i = 0; i < data->number_of_philosophers; i++)
		{
			pthread_mutex_lock(&data->death_mutex);
			if (get_time_in_ms(data) - data->philosophers[i].last_meal_time > data->time_to_die)
			{
				print_message(&data->philosophers[i], "died");
				data->simulation_end = 1;
				pthread_mutex_unlock(&data->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_mutex);
		}
		usleep(100);
	}
	return (NULL);
}

int	is_valid_number(const char *str)
{
	int	i;

	i = (str[0] == '-' || str[0] == '+') ? 1 : 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		fprintf(stderr, "Error: Invalid number of arguments.\n");
		return (0);
	}
	for (int i = 1; i < argc; i++)
	{
		if (!is_valid_number(argv[i]))
		{
			fprintf(stderr, "Error: Argument %d is not a valid number.\n", i);
			return (0);
		}
		long long arg_value = atoll(argv[i]);
		if (arg_value <= 0 || arg_value > 4294967295)
		{
			fprintf(stderr, "Error: Argument %d is out of valid range.\n", i);
			return (0);
		}
	}
	data->number_of_philosophers = atoi(argv[1]);
	if (data->number_of_philosophers <= 0)
	{
		fprintf(stderr, "Error: Number of philosophers must be greater than 0.\n");
		return (0);
	}
	data->time_to_die = atoll(argv[2]);
	data->time_to_eat = atoll(argv[3]);
	data->time_to_sleep = atoll(argv[4]);
	data->number_of_meals = (argc == 6) ? atoi(argv[5]) : -1;
	return (1);
}



void	cleanup(t_data *data, pthread_t *philosophers)
{
	free(philosophers);
	for (int i = 0; i < data->number_of_philosophers; i++)
		pthread_mutex_destroy(&data->philosophers[i].left_fork);
	free(data->philosophers);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}
int initialize_simulation(t_data *data)
{
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);

	data->philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
	if (!data->philosophers)
		return (1);

	for (int i = 0; i < data->number_of_philosophers; i++)
	{
		// Initialize philosopher attributes
		data->philosophers[i].id = i + 1;
		data->philosophers[i].last_meal_time = get_time_in_ms(data);
		data->philosophers[i].meals_eaten = 0;
		pthread_mutex_init(&data->philosophers[i].left_fork, NULL); // Initialize left fork mutex

		// Assign right fork correctly: the last philosopher shares the first philosopher's left fork
		if (i == data->number_of_philosophers - 1)
			data->philosophers[i].right_fork = &data->philosophers[0].left_fork;
		else
			data->philosophers[i].right_fork = &data->philosophers[i + 1].left_fork;

		data->philosophers[i].data = data;  // Link philosopher to shared data
	}
	return (0);
}


int main(int argc, char **argv)
{
	t_data		data;
	pthread_t	*philosophers;
	pthread_t	monitor;

	memset(&data, 0, sizeof(t_data));
	gettimeofday(&(struct timeval){}, NULL);
	data.start_time = get_time_in_ms(&data);

	// Argument Parsing Error Handling
	if (!parse_arguments(argc, argv, &data))
		return (1);  // Return non-zero status if argument parsing fails

	// Simulation Initialization Error Handling
	if (initialize_simulation(&data))
	{
		fprintf(stderr, "Error: Failed to initialize simulation.\n");
		return (1);  // Return non-zero status if initialization fails
	}

	philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers);
	if (!philosophers)
	{
		free(data.philosophers);
		fprintf(stderr, "Error: Memory allocation failed for philosophers.\n");
		return (1);  // Return non-zero status if malloc fails
	}

	// Start philosopher threads
	for (int i = 0; i < data.number_of_philosophers; i++)
		pthread_create(&philosophers[i], NULL, philosopher_life, &data.philosophers[i]);

	// Start death monitor thread
	pthread_create(&monitor, NULL, death_monitor, &data);

	// Wait for philosopher threads to finish
	for (int i = 0; i < data.number_of_philosophers; i++)
		pthread_join(philosophers[i], NULL);

	// Wait for the monitor thread to finish
	pthread_join(monitor, NULL);

	// Cleanup resources
	cleanup(&data, philosophers);
	return (0);
}


