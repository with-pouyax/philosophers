#include "philo.h"

int is_valid_number(const char *str)
{
    int i = 0;
    
    // Check for an optional sign
    if (str[i] == '+' || str[i] == '-')
        i++;
    
    // Ensure the rest of the string is digits
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;  // Not a valid number
        i++;
    }

    return 1;  // Valid number
}

// Setter for simulation_end
void set_simulation_end(t_data *data, int value)
{
    pthread_mutex_lock(&data->simulation_mutex);
    data->simulation_end = value;
    pthread_mutex_unlock(&data->simulation_mutex);
}

// Getter for simulation_end
int get_simulation_end(t_data *data)
{
    int value;
    pthread_mutex_lock(&data->simulation_mutex);
    value = data->simulation_end;
    pthread_mutex_unlock(&data->simulation_mutex);
    return value;
}

// Setter for last_meal_time
void set_last_meal_time(t_philosopher *philo, long long time)
{
    pthread_mutex_lock(&philo->data->simulation_mutex);
    philo->last_meal_time = time;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
}

// Getter for last_meal_time
long long get_last_meal_time(t_philosopher *philo)
{
    long long time;
    pthread_mutex_lock(&philo->data->simulation_mutex);
    time = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
    return time;
}

// Setter for meals_eaten
void set_meals_eaten(t_philosopher *philo, int meals)
{
    pthread_mutex_lock(&philo->data->simulation_mutex);
    philo->meals_eaten = meals;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
}

// Getter for meals_eaten
int get_meals_eaten(t_philosopher *philo)
{
    int meals;
    pthread_mutex_lock(&philo->data->simulation_mutex);
    meals = philo->meals_eaten;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
    return meals;
}

// Setter for ate_enough
void set_ate_enough(t_philosopher *philo, int value)
{
    pthread_mutex_lock(&philo->data->simulation_mutex);
    philo->ate_enough = value;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
}

// Getter for ate_enough
int get_ate_enough(t_philosopher *philo)
{
    int value;
    pthread_mutex_lock(&philo->data->simulation_mutex);
    value = philo->ate_enough;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
    return value;
}

// Get current time in ms
long long get_time_in_ms(t_data *data)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - data->start_time;
}

// Custom usleep function that checks for simulation_end
void custom_usleep(long long time_in_ms, t_data *data)
{
    long long start_time = get_time_in_ms(data);
    while (get_time_in_ms(data) - start_time < time_in_ms)
    {
        if (get_simulation_end(data))
            break;
        usleep(100);
    }
}

// Print a message with mutex protection
void print_message(t_philosopher *philo, char *message)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    if (!get_simulation_end(philo->data))
        printf("%lld %d %s\n", get_time_in_ms(philo->data), philo->id, message);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

// Philosopher's eating behavior
void philosopher_eat(t_philosopher *philo)
{
    if (get_simulation_end(philo->data))
        return;

    if (philo->data->number_of_philosophers == 1)
    {
        pthread_mutex_lock(&philo->left_fork->mutex);
        print_message(philo, "has taken a fork");
        custom_usleep(philo->data->time_to_die, philo->data);
        print_message(philo, "died");
        pthread_mutex_unlock(&philo->left_fork->mutex);
        return;
    }

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

    pthread_mutex_unlock(&philo->right_fork->mutex);
    pthread_mutex_unlock(&philo->left_fork->mutex);

    set_meals_eaten(philo, get_meals_eaten(philo) + 1);

    if (philo->data->number_of_meals != -1 && get_meals_eaten(philo) >= philo->data->number_of_meals)
        set_ate_enough(philo, 1);
}

// Philosopher's life cycle
void *philosopher_life(void *philosopher)
{
    t_philosopher *philo = (t_philosopher *)philosopher;

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

// Monitor for philosopher deaths
void *death_monitor(void *data_void)
{
    t_data *data = (t_data *)data_void;

    while (!get_simulation_end(data))
    {
        for (int i = 0; i < data->number_of_philosophers; i++)
        {
            if (get_time_in_ms(data) - get_last_meal_time(&data->philosophers[i]) > data->time_to_die)
            {
                print_message(&data->philosophers[i], "died");
                set_simulation_end(data, 1);
                return (NULL);
            }
        }
        usleep(100);
    }
    return (NULL);
}

// Monitor to check if all philosophers have eaten enough
void *meal_monitor(void *data_void)
{
    t_data *data = (t_data *)data_void;

    while (!get_simulation_end(data))
    {
        int all_ate_enough = 1;

        for (int i = 0; i < data->number_of_philosophers; i++)
        {
            if (!get_ate_enough(&data->philosophers[i]))
            {
                all_ate_enough = 0;
                break;
            }
        }

        if (all_ate_enough)
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

	// Skip whitespaces
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	
	// Handle sign if present
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	
	// Convert the string to long long
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LLONG_MAX / 10) || (result == LLONG_MAX / 10 && (str[i] - '0') > (LLONG_MAX % 10)))
		{
			if (sign == 1)
				return (-1); // Overflow for positive numbers
			else
				return (0);  // Overflow for negative numbers
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}

	return (result * sign);
}


int parse_arguments(int argc, char **argv, t_data *data)
{
    long long arg_value;

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
        arg_value = ft_atoll(argv[i]);  // Replace atoll with ft_atoll
        if (arg_value <= 0 || arg_value > 4294967295)
        {
            fprintf(stderr, "Error: Argument %d is out of valid range (0 < value <= 4294967295).\n", i);
            return (0);
        }
    }
    
    // Assign arguments to data structure
    data->number_of_philosophers = atoi(argv[1]);
    data->time_to_die = ft_atoll(argv[2]);
    data->time_to_eat = ft_atoll(argv[3]);
    data->time_to_sleep = ft_atoll(argv[4]);
    data->number_of_meals = (argc == 6) ? atoi(argv[5]) : -1;

    return (1);
}


int initialize_simulation(t_data *data)
{
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->simulation_mutex, NULL);

    data->philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
    data->forks = malloc(sizeof(t_fork) * data->number_of_philosophers);
    if (!data->philosophers || !data->forks)
        return (1);

    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].meals_eaten = 0;
        data->philosophers[i].ate_enough = 0;
        data->philosophers[i].last_meal_time = get_time_in_ms(data);
        data->philosophers[i].left_fork = &data->forks[i];
        pthread_mutex_init(&data->forks[i].mutex, NULL);
        if (i == data->number_of_philosophers - 1)
            data->philosophers[i].right_fork = &data->forks[0];
        else
            data->philosophers[i].right_fork = &data->forks[i + 1];
        data->philosophers[i].data = data;
    }
    return (0);
}

void cleanup(t_data *data, pthread_t *philosophers)
{
    free(philosophers);
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_mutex_destroy(&data->forks[i].mutex);
    }
    free(data->forks);
    free(data->philosophers);
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->simulation_mutex);
}

int main(int argc, char **argv)
{
    t_data          data;
    pthread_t       *philosophers;
    pthread_t       monitor, meal_checker;

    memset(&data, 0, sizeof(t_data));
    data.start_time = get_time_in_ms(&data);

    if (!parse_arguments(argc, argv, &data))
        return (1);
    if (initialize_simulation(&data))
    {
        fprintf(stderr, "Error: Failed to initialize simulation.\n");
        return (1);
    }

    philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers);
    if (!philosophers)
    {
        free(data.philosophers);
        return (1);
    }

    for (int i = 0; i < data.number_of_philosophers; i++)
        pthread_create(&philosophers[i], NULL, philosopher_life, &data.philosophers[i]);

    pthread_create(&monitor, NULL, death_monitor, &data);
    if (data.number_of_meals != -1)
        pthread_create(&meal_checker, NULL, meal_monitor, &data);

    for (int i = 0; i < data.number_of_philosophers; i++)
        pthread_join(philosophers[i], NULL);

    pthread_join(monitor, NULL);
    if (data.number_of_meals != -1)
        pthread_join(meal_checker, NULL);

    cleanup(&data, philosophers);
    return (0);
}
