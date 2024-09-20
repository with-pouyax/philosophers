#include "philo.h"

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	set_simulation_end(t_data *data, int value)
{
	pthread_mutex_lock(&data->output_mutex);
	data->simulation_end = value;
	pthread_mutex_unlock(&data->output_mutex);
}

int	get_simulation_end(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->output_mutex);
	value = data->simulation_end;
	pthread_mutex_unlock(&data->output_mutex);
	return (value);
}

void	set_last_meal_time(t_philosopher *philo, long long time)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
}

long long get_last_meal_time(t_philosopher *philo)
{
	long long	time;

	pthread_mutex_lock(&philo->last_meal_mutex);
	time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (time);
}

void	set_meals_eaten(t_philosopher *philo, int meals)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten = meals;
	pthread_mutex_unlock(&philo->meal_mutex);
}

int	get_meals_eaten(t_philosopher *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->meal_mutex);
	meals = philo->meals_eaten;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (meals);
}

void	set_ate_enough(t_philosopher *philo, int value)
{
	pthread_mutex_lock(&philo->ate_enough_mutex);
	philo->ate_enough = value;
	pthread_mutex_unlock(&philo->ate_enough_mutex);
}

int	get_ate_enough(t_philosopher *philo)
{
	int	value;

	pthread_mutex_lock(&philo->ate_enough_mutex);
	value = philo->ate_enough;
	pthread_mutex_unlock(&philo->ate_enough_mutex);
	return (value);
}

long long get_time_in_ms(t_data *data)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - data->start_time;
}

void	custom_usleep(long long time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_time_in_ms(data);
	while (get_time_in_ms(data) - start_time < time_in_ms)
	{
		if (get_simulation_end(data))
			break ;
		usleep(100);
	}
}

void	print_message(t_philosopher *philo, char *message)
{
	pthread_mutex_lock(&philo->data->output_mutex);
	if (!philo->data->simulation_end)
		printf("%lld %d %s\n", get_time_in_ms(philo->data), philo->id, message);
	pthread_mutex_unlock(&philo->data->output_mutex);
}

void	philosopher_eat(t_philosopher *philo)
{
	if (get_simulation_end(philo->data))
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_message(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_message(philo, "has taken a fork");
	}
	set_last_meal_time(philo, get_time_in_ms(philo->data));
	print_message(philo, "is eating");
	custom_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	set_meals_eaten(philo, get_meals_eaten(philo) + 1);
	if (philo->data->num_meals != -1 && get_meals_eaten(philo) >= philo->data->num_meals)
		set_ate_enough(philo, 1);
}

void	*philosopher_life(void *philosopher)
{
    t_philosopher	*philo;

    philo = (t_philosopher *)philosopher;

    // Wait for the start signal to begin the routine
    pthread_mutex_lock(&philo->data->start_mutex);
    pthread_mutex_unlock(&philo->data->start_mutex);

    if (philo->data->num_philosophers % 2 == 0 && philo->id == 1) // If number of philosophers is even and current philosopher is the 
																  // first one, sleep for 1 ms.
    {
        usleep(100); 
    }

    while (!get_simulation_end(philo->data))
    {
        philosopher_eat(philo);
        print_message(philo, "is sleeping");
        custom_usleep(philo->data->time_to_sleep, philo->data);
        print_message(philo, "is thinking");
    }
    return (NULL);
}

void	*god(void *data_void)
{
	t_data	*data;
	int		i;
	int		all_ate_enough;

	data = (t_data *)data_void;
	while (!get_simulation_end(data))
	{
		all_ate_enough = 1;
		i = 0;
		while (i < data->num_philosophers)
		{
			if (get_time_in_ms(data) - get_last_meal_time(&data->philosophers[i]) > data->time_to_die)
			{
				print_message(&data->philosophers[i], "died");
				set_simulation_end(data, 1);
				return (NULL);
			}
			if (!get_ate_enough(&data->philosophers[i]))
			{
				all_ate_enough = 0;
			}
			i++;
		}
		if (data->num_meals != -1 && all_ate_enough)
		{
			set_simulation_end(data, 1);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LLONG_MAX / 10) || (result == LLONG_MAX / 10 && (str[i] - '0') > (LLONG_MAX % 10)))
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	long long	arg_value;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error: Invalid number of arguments.\n", 36);
		return (0);
	}
	for (int i = 1; i < argc; i++)
	{
		if (!is_valid_number(argv[i]))
		{
			write(2, "Error: Invalid argument.\n", 25);
			return (0);
		}
		arg_value = ft_atoll(argv[i]);
		if (arg_value <= 0 || arg_value > 4294967295)
		{
			write(2, "Error: Argument out of valid range.\n", 35);
			return (0);
		}
	}
	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	data->num_meals = (argc == 6) ? atoi(argv[5]) : -1;
	return (1);
}

int	initialize_simulation(t_data *data)
{
    pthread_mutex_init(&data->output_mutex, NULL);
    pthread_mutex_init(&data->start_mutex, NULL);

    // Lock the start mutex here so all threads wait for it to unlock.
    pthread_mutex_lock(&data->start_mutex);

    for (int i = 0; i < data->num_philosophers; i++)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].meals_eaten = 0;
        data->philosophers[i].ate_enough = 0;
        data->philosophers[i].last_meal_time = 0;  // Initializing last_meal_time to 0
        data->philosophers[i].left_fork = &data->forks[i];
        pthread_mutex_init(&data->forks[i].mutex, NULL);
        data->philosophers[i].right_fork = (i == data->num_philosophers - 1)
            ? &data->forks[0] : &data->forks[i + 1];
        pthread_mutex_init(&data->philosophers[i].meal_mutex, NULL);
        pthread_mutex_init(&data->philosophers[i].last_meal_mutex, NULL);
        pthread_mutex_init(&data->philosophers[i].ate_enough_mutex, NULL);
        data->philosophers[i].data = data;
    }
    return (0);
}

void	cleanup(t_data *data)
{
	for (int i = 0; i < data->num_philosophers; i++)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		pthread_mutex_destroy(&data->philosophers[i].meal_mutex);
		pthread_mutex_destroy(&data->philosophers[i].last_meal_mutex);
		pthread_mutex_destroy(&data->philosophers[i].ate_enough_mutex);
	}
	pthread_mutex_destroy(&data->output_mutex);
	pthread_mutex_destroy(&data->start_mutex);
}

void	*single_philosopher(void *philosopher)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philosopher;

	printf("%lld %d has taken a fork\n", get_time_in_ms(philo->data), philo->id);  
	custom_usleep(philo->data->time_to_die, philo->data);  
	printf("%lld %d died\n", get_time_in_ms(philo->data), philo->id);  
	
	return (NULL);
}

int	main(int argc, char **argv)
{
    t_data		data;
    pthread_t	philosophers[BUFFER_SIZE]; 
    pthread_t	god_thread;
    int			i;

    memset(&data, 0, sizeof(t_data));
    
    // Parse arguments to initialize the data (philosophers, times, etc.)
    if (!parse_arguments(argc, argv, &data))
        return (1);
    
    // Initialize simulation (mutexes, philosophers, etc.)
    if (initialize_simulation(&data)) 
        return (1);

    // Special case for a single philosopher
    if (data.num_philosophers == 1)
    {
        // Set start time to 0 here before starting the philosopher's routine
        data.start_time = get_time_in_ms(&data); 
        set_last_meal_time(&data.philosophers[0], data.start_time);

        pthread_create(&philosophers[0], NULL, single_philosopher, &data.philosophers[0]);
        pthread_join(philosophers[0], NULL);  
        
        cleanup(&data);  
        return (0);
    }

    // Create philosopher threads for the general case (more than 1 philosopher)
    i = 0;
    while (i < data.num_philosophers)
    {
        pthread_create(&philosophers[i], NULL, philosopher_life, &data.philosophers[i]); // Create philosopher threads
        i++;
    }

    // Set the start time after all threads have been created for synchronization
    data.start_time = get_time_in_ms(&data); 

    // Once all threads are created, set their last meal time to the start time
    for (i = 0; i < data.num_philosophers; i++)
    {
        set_last_meal_time(&data.philosophers[i], data.start_time);
    }

    // Unlock start_mutex to let all threads start at the same time
    pthread_mutex_unlock(&data.start_mutex);

    // Create the 'god' thread to monitor philosophers' deaths and meals
    pthread_create(&god_thread, NULL, god, &data);

    // Wait for all philosopher threads to finish
    i = 0;
    while (i < data.num_philosophers)
    {
        pthread_join(philosophers[i], NULL);
        i++;
    }

    // Wait for the god thread to finish
    pthread_join(god_thread, NULL);

    // Cleanup and free resources
    cleanup(&data);
    return (0);
}