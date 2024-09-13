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
    int value; // Declare a variable to store the value
    pthread_mutex_lock(&data->simulation_mutex); // Lock the simulation mutex
    value = data->simulation_end; // Get the value
    pthread_mutex_unlock(&data->simulation_mutex);
    return value; // Return the value
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
    struct timeval tv; // struct timeval is defined in sys/time.h and contains tv_sec and tv_usec fields.
                       // tv_sec is the number of seconds since the Epoch, which is 00:00:00 UTC on 1 January 1970.
                       // tv_usec is the number of microseconds (1/1,000,000 of a second) since the last full second.
    gettimeofday(&tv, NULL); // gettimeofday is defined in sys/time.h and fills the tv struct with the current time. 
                             // The second argument is a timezone struct, which is not used here.
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - data->start_time; // by tv.tv_sec * 1000 we convert seconds to milliseconds
                                                                            // by tv.tv_usec / 1000 we convert microseconds to milliseconds
                                                                            // by subtracting data->start_time we get the time since the start of the simulation
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
    if (get_simulation_end(philo->data)) // check if the simulation ended
        return; // if it did, return

    if (philo->data->number_of_philosophers == 1) // if there is only one philosopher
    {
        pthread_mutex_lock(&philo->left_fork->mutex); // take the fork
        print_message(philo, "has taken a fork"); // print message that the philosopher has taken a fork
        custom_usleep(philo->data->time_to_die, philo->data); // sleep for the specified time
        print_message(philo, "died"); // print message that the philosopher died
        pthread_mutex_unlock(&philo->left_fork->mutex); // unlock the fork
        return; // return
    }

    if (philo->id % 2 == 0) // if the philosopher id is even
    {
        pthread_mutex_lock(&philo->right_fork->mutex); // take the right fork
        print_message(philo, "has taken a fork"); // print message that the philosopher has taken a fork
        pthread_mutex_lock(&philo->left_fork->mutex); // take the left fork
        print_message(philo, "has taken a fork"); // print message that the philosopher has taken a fork
    }
    else // if the philosopher id is odd
    {
        pthread_mutex_lock(&philo->left_fork->mutex); // take the left fork first
        print_message(philo, "has taken a fork"); // print message that the philosopher has taken a fork
        pthread_mutex_lock(&philo->right_fork->mutex); // take the right fork
        print_message(philo, "has taken a fork"); // print message that the philosopher has taken a fork
    }
    set_last_meal_time(philo, get_time_in_ms(philo->data)); // after taking the forks, set the last meal time to the current time
    print_message(philo, "is eating"); // print message that the philosopher is eating
    custom_usleep(philo->data->time_to_eat, philo->data); // sleep for the specified time after eating
    if (philo->id % 2 == 0) 
    {
        pthread_mutex_unlock(&philo->right_fork->mutex); // unlock the right fork first if ID is even
        pthread_mutex_unlock(&philo->left_fork->mutex); // then unlock the left fork
    } 
    else 
    {
        pthread_mutex_unlock(&philo->left_fork->mutex); // unlock the left fork first if ID is odd
        pthread_mutex_unlock(&philo->right_fork->mutex); // then unlock the right fork
    }
    set_meals_eaten(philo, get_meals_eaten(philo) + 1); // increment the meals eaten by the philosopher
    if (philo->data->number_of_meals != -1 && get_meals_eaten(philo) >= philo->data->number_of_meals) // if the number of meals is set and the
                                                                                                      // philosopher has eaten enough meals
                                                                                                      // set ate_enough to 1
        set_ate_enough(philo, 1);
}

// Philosopher's life cycle
void *philosopher_life(void *philosopher)
{
    t_philosopher *philo = (t_philosopher *)philosopher;

    while (!get_simulation_end(philo->data)) // loop until the simulation ends
    {
        philosopher_eat(philo); // first the philosopher eats 
        if (get_simulation_end(philo->data)) // check if the simulation ended while eating, it can end after eating if the philosopher ate enough
                                             // or if the philosopher died
            break;
        print_message(philo, "is sleeping"); // print message that the philosopher is sleeping
        custom_usleep(philo->data->time_to_sleep, philo->data); // sleep for the specified time
        if (get_simulation_end(philo->data)) // check if the simulation ended while sleeping
            break;
        print_message(philo, "is thinking"); // print message that the philosopher is thinking
    }
    return (NULL); // return NULL to indicate the end of the thread
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
    pthread_mutex_init(&data->print_mutex, NULL); // Initialize print_mutex
    pthread_mutex_init(&data->simulation_mutex, NULL); // Initialize simulation_mutex

    data->philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers); // Allocate memory for philosophers array
    data->forks = malloc(sizeof(t_fork) * data->number_of_philosophers); // Allocate memory for forks array
    if (!data->philosophers || !data->forks) // Check if memory allocation was successful
        return (1);

    for (int i = 0; i < data->number_of_philosophers; i++) // loop through the philosophers array
    {
        data->philosophers[i].id = i + 1; // Set philosopher id
        data->philosophers[i].meals_eaten = 0; // Set meals_eaten to 0
        data->philosophers[i].ate_enough = 0; // Set ate_enough to 0
        data->philosophers[i].last_meal_time = get_time_in_ms(data); // Set last_meal_time to the current time
        data->philosophers[i].left_fork = &data->forks[i]; // Set left_fork to the current fork
        pthread_mutex_init(&data->forks[i].mutex, NULL); // Initialize the fork mutex
        if (i == data->number_of_philosophers - 1) // If we are at the last philosopher
            data->philosophers[i].right_fork = &data->forks[0]; // Set right_fork to the first fork
        else
            data->philosophers[i].right_fork = &data->forks[i + 1]; // other than that, set right_fork to the next fork
        data->philosophers[i].data = data; // Set philosopher data to the simulation data
    }
    return (0); // Return 0 to indicate success
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
    int             i;

    memset(&data, 0, sizeof(t_data)); // Initialize all fields in data to 0
    data.start_time = get_time_in_ms(&data); // Set start time

    if (!parse_arguments(argc, argv, &data)) // we check if the arguments are valid
        return (1);
    if (initialize_simulation(&data)) // we initialize the simulation 
    {
        fprintf(stderr, "Error: Failed to initialize simulation.\n");
        return (1);
    }

    philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers); // Allocate memory for philosophers array
    if (!philosophers)
    {
        free(data.philosophers); // Free philosophers array if memory allocation failed
        return (1); 
    }

    // Using while loop instead of for loop
    i = 0; 
    while (i < data.number_of_philosophers) // loop through the philosophers array and for each philosopher create a thread
    {
        pthread_create(&philosophers[i], NULL, philosopher_life, &data.philosophers[i]); // Create a thread for each philosopher and
                                                                                         // run the philosopher_life function 
                                                                                         // pass the philosopher data as an argument
        i++;
    }

    pthread_create(&monitor, NULL, death_monitor, &data); // create a thread for the death monitor
    if (data.number_of_meals != -1) // if the number of meals is set
        pthread_create(&meal_checker, NULL, meal_monitor, &data); // create a thread for the meal monitor

    // Using while loop instead of for loop
    i = 0; 
    while (i < data.number_of_philosophers) // loop through the philosophers array and wait for each philosopher thread to finish
    {
        pthread_join(philosophers[i], NULL); // wait for the philosopher thread to finish
        i++;
    }

    pthread_join(monitor, NULL); // wait for the death monitor thread to finish
    if (data.number_of_meals != -1) // if the number of meals is set
        pthread_join(meal_checker, NULL); // wait for the meal monitor thread to finish

    cleanup(&data, philosophers); // clean up the simulation data and free memory
    return (0);
}

