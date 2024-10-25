/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyaximac <pouyaximac@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:52:02 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/10/25 23:01:59 by pouyaximac       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo.h"

long long   ft_atoll(const char *str)
{
    long long   result;
    int         sign;
    int         i;

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

int is_valid_number(const char *str)
{
    int i;

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

void set_simulation_end(t_data *data, int value)
{
    pthread_mutex_lock(&data->output_mutex);
    data->simulation_end = value;
    pthread_mutex_unlock(&data->output_mutex);
}



int get_simulation_end(t_data *data)
{
    int value;

    pthread_mutex_lock(&data->output_mutex);
    value = data->simulation_end;
    pthread_mutex_unlock(&data->output_mutex);
    return value;
}



void set_last_meal_time(t_philosopher *philo, long long time)
{
    pthread_mutex_lock(&philo->philo_mutex);
    philo->last_meal_time = time;
    pthread_mutex_unlock(&philo->philo_mutex);
}

long long get_last_meal_time(t_philosopher *philo)
{
    long long time;

    pthread_mutex_lock(&philo->philo_mutex);
    time = philo->last_meal_time;
    pthread_mutex_unlock(&philo->philo_mutex);
    return time;
}


void    set_meals_eaten(t_philosopher *philo, int meals)
{
    pthread_mutex_lock(&philo->philo_mutex);
    philo->meals_eaten = meals;
    pthread_mutex_unlock(&philo->philo_mutex);
}

int get_meals_eaten(t_philosopher *philo)
{
    int meals;

    pthread_mutex_lock(&philo->philo_mutex);
    meals = philo->meals_eaten;
    pthread_mutex_unlock(&philo->philo_mutex);
    return (meals);
}

void    set_ate_enough(t_philosopher *philo, int value)
{
    pthread_mutex_lock(&philo->philo_mutex);
    philo->ate_enough = value;
    pthread_mutex_unlock(&philo->philo_mutex);
}

int get_ate_enough(t_philosopher *philo)
{
    int value;

    pthread_mutex_lock(&philo->philo_mutex);
    value = philo->ate_enough;
    pthread_mutex_unlock(&philo->philo_mutex);
    return (value);
}

long long get_time_in_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void custom_usleep(long long time_in_ms, t_data *data)
{
    long long start_time = get_time_in_ms();
    long long elapsed_time;

    while ((elapsed_time = get_time_in_ms() - start_time) < time_in_ms)
    {
        if (get_simulation_end(data))
            break;
        usleep(500);  // Sleep for 500 microseconds to allow frequent checks
    }
}








void print_message(t_philosopher *philo, char *message)
{
    pthread_mutex_lock(&philo->data->output_mutex);
    if (!philo->data->simulation_end)
        printf("%lld %d %s\n", get_time_in_ms() - philo->data->start_time, philo->id, message);
    pthread_mutex_unlock(&philo->data->output_mutex);
}



void philosopher_eat(t_philosopher *philo)
{
    if (get_simulation_end(philo->data))
        return;

    // All philosophers pick up the right fork first, then the left fork
    pthread_mutex_lock(&philo->right_fork->mutex);
    print_message(philo, "has taken a fork");
    pthread_mutex_lock(&philo->left_fork->mutex);
    print_message(philo, "has taken a fork");

    set_last_meal_time(philo, get_time_in_ms());
    print_message(philo, "is eating");
    custom_usleep(philo->data->time_to_eat, philo->data);

    pthread_mutex_unlock(&philo->left_fork->mutex);
    pthread_mutex_unlock(&philo->right_fork->mutex);

    set_meals_eaten(philo, get_meals_eaten(philo) + 1);
    if (philo->data->num_meals != -1 && get_meals_eaten(philo) >= philo->data->num_meals)
        set_ate_enough(philo, 1);
}












void *philosopher_life(void *philosopher)
{
    t_philosopher *philo;

    philo = (t_philosopher *)philosopher;
    pthread_mutex_lock(&philo->data->start_mutex);
    pthread_mutex_unlock(&philo->data->start_mutex);

    // Introduce initial delays based on philosopher ID
    if (philo->id % 2 == 0)
        usleep(philo->data->time_to_eat * 500);  // Even-numbered philosophers delay
    else if (philo->data->num_philosophers > 100 && philo->id % 3 == 0)
        usleep(philo->data->time_to_eat * 1000);  // Additional staggering for large numbers

    set_last_meal_time(philo, get_time_in_ms());

    while (!get_simulation_end(philo->data))
    {
        philosopher_eat(philo);
        print_message(philo, "is sleeping");
        custom_usleep(philo->data->time_to_sleep, philo->data);
        print_message(philo, "is thinking");

        // Include death checking in philosopher thread
        if (get_time_in_ms() - get_last_meal_time(philo) >= philo->data->time_to_die)
        {
            print_message(philo, "died");
            set_simulation_end(philo->data, 1);
            break;
        }
    }
    return (NULL);
}









int parse_arguments(int argc, char **argv, t_data *data)
{
    long long   arg_value;
    int         i;

    if (argc < 5 || argc > 6)
    {
        write(2, "Error: Invalid number of arguments.\n", 36);
        return (0);
    }
    i = 1;
    while (i < argc)
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
        i++;
    }
    data->num_philosophers = atoi(argv[1]);
    data->time_to_die = ft_atoll(argv[2]);
    data->time_to_eat = ft_atoll(argv[3]);
    data->time_to_sleep = ft_atoll(argv[4]);
    if (argc == 6)
        data->num_meals = atoi(argv[5]);
    else
        data->num_meals = -1;
    return (1);
}

int initialize_simulation(t_data *data)
{
    int i;

    if (pthread_mutex_init(&data->output_mutex, NULL) != 0) {
        fprintf(stderr, "Error: Failed to initialize output mutex.\n");
        return (1);
    }
    if (pthread_mutex_init(&data->start_mutex, NULL) != 0) {
        fprintf(stderr, "Error: Failed to initialize start mutex.\n");
        pthread_mutex_destroy(&data->output_mutex);
        return (1);
    }

    i = 0;
    while (i < data->num_philosophers)
    {
        data->forks[i].id = i + 1;  // Assign fork IDs

        data->philosophers[i].id = i + 1;
        data->philosophers[i].meals_eaten = 0;
        data->philosophers[i].ate_enough = 0;
        data->philosophers[i].last_meal_time = 0;
        data->philosophers[i].left_fork = &data->forks[i];
        if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0) {
            fprintf(stderr, "Error: Failed to initialize fork mutex %d.\n", i + 1);
            // Cleanup already initialized mutexes
            int j;
            for (j = 0; j < i; j++) {
                pthread_mutex_destroy(&data->forks[j].mutex);
                pthread_mutex_destroy(&data->philosophers[j].philo_mutex);
            }
            pthread_mutex_destroy(&data->output_mutex);
            pthread_mutex_destroy(&data->start_mutex);
            return (1);
        }
        if (pthread_mutex_init(&data->philosophers[i].philo_mutex, NULL) != 0) {
            fprintf(stderr, "Error: Failed to initialize philosopher mutex %d.\n", i + 1);
            // Cleanup
            pthread_mutex_destroy(&data->forks[i].mutex);
            int j;
            for (j = 0; j < i; j++) {
                pthread_mutex_destroy(&data->forks[j].mutex);
                pthread_mutex_destroy(&data->philosophers[j].philo_mutex);
            }
            pthread_mutex_destroy(&data->output_mutex);
            pthread_mutex_destroy(&data->start_mutex);
            return (1);
        }
        if (i == data->num_philosophers - 1)
            data->philosophers[i].right_fork = &data->forks[0];
        else
            data->philosophers[i].right_fork = &data->forks[i + 1];
        data->philosophers[i].data = data;
        i++;
    }
    return (0);
}




void cleanup(t_data *data)
{
    int i;

    i = 0;
    while (i < data->num_philosophers)
    {
        if (pthread_mutex_destroy(&data->forks[i].mutex) != 0)
            fprintf(stderr, "Error: Failed to destroy fork mutex %d.\n", i + 1);
        if (pthread_mutex_destroy(&data->philosophers[i].philo_mutex) != 0)
            fprintf(stderr, "Error: Failed to destroy philosopher mutex %d.\n", i + 1);
        i++;
    }
    if (pthread_mutex_destroy(&data->output_mutex) != 0)
        fprintf(stderr, "Error: Failed to destroy output mutex.\n");
    if (pthread_mutex_destroy(&data->start_mutex) != 0)
        fprintf(stderr, "Error: Failed to destroy start mutex.\n");
}


void *single_philosopher(void *philosopher)
{
    t_philosopher *philo;

    philo = (t_philosopher *)philosopher;
    pthread_mutex_lock(&philo->data->start_mutex);  // Lock to synchronize with main
    pthread_mutex_unlock(&philo->data->start_mutex);  // Unlock immediately

    printf("%lld %d has taken a fork\n", get_time_in_ms() - philo->data->start_time, philo->id);
    custom_usleep(philo->data->time_to_die, philo->data);
    printf("%lld %d died\n", get_time_in_ms() - philo->data->start_time, philo->id);
    return (NULL);
}


int main(int argc, char **argv)
{
    t_data      data;
    pthread_t   philosophers[BUFFER_SIZE];
    int         i;
    int         all_ate_enough;
    int         ret;

    memset(&data, 0, sizeof(t_data));
    if (!parse_arguments(argc, argv, &data))
        return (1);
    if (initialize_simulation(&data))
        return (1);
    data.start_time = get_time_in_ms(); // Initialize start_time here

    if (pthread_mutex_lock(&data.start_mutex) != 0) {
        fprintf(stderr, "Error: Failed to lock start mutex.\n");
        cleanup(&data);
        return (1);
    }

    if (data.num_philosophers == 1)
    {
        if (pthread_mutex_unlock(&data.start_mutex) != 0) {
            fprintf(stderr, "Error: Failed to unlock start mutex.\n");
            cleanup(&data);
            return (1);
        }
        set_last_meal_time(&data.philosophers[0], data.start_time);
        ret = pthread_create(&philosophers[0], NULL, single_philosopher, &data.philosophers[0]);
        if (ret != 0) {
            fprintf(stderr, "Error: Failed to create philosopher thread.\n");
            cleanup(&data);
            return (1);
        }
        pthread_join(philosophers[0], NULL);
        cleanup(&data);
        return (0);
    }

    i = 0;
    while (i < data.num_philosophers)
    {
        set_last_meal_time(&data.philosophers[i], data.start_time);
        ret = pthread_create(&philosophers[i], NULL, philosopher_life, &data.philosophers[i]);
        if (ret != 0)
        {
            fprintf(stderr, "Error: Failed to create philosopher thread %d.\n", i + 1);
            set_simulation_end(&data, 1);
            // Join already created threads
            int j = 0;
            while (j < i)
            {
                pthread_join(philosophers[j], NULL);
                j++;
            }
            cleanup(&data);
            return (1);
        }
        i++;
    }

    if (pthread_mutex_unlock(&data.start_mutex) != 0) {
        fprintf(stderr, "Error: Failed to unlock start mutex.\n");
        set_simulation_end(&data, 1);
        // Join all threads
        i = 0;
        while (i < data.num_philosophers)
        {
            pthread_join(philosophers[i], NULL);
            i++;
        }
        cleanup(&data);
        return (1);
    }

   while (!get_simulation_end(&data))
{
    all_ate_enough = 1;
    i = 0;
    while (i < data.num_philosophers)
    {
        long long time_since_last_meal = get_time_in_ms() - get_last_meal_time(&data.philosophers[i]);

        if (time_since_last_meal >= data.time_to_die)
        {
            print_message(&data.philosophers[i], "died");
            set_simulation_end(&data, 1);
            break;
        }
        if (!get_ate_enough(&data.philosophers[i]))
            all_ate_enough = 0;
        i++;
    }
    if (data.num_meals != -1 && all_ate_enough)
    {
        set_simulation_end(&data, 1);
        break;
    }
    usleep(500);  // Sleep for 500 microseconds to allow frequent checks
}


    // Join philosopher threads before cleanup
    i = 0;
    while (i < data.num_philosophers)
    {
        pthread_join(philosophers[i], NULL);
        i++;
    }

    cleanup(&data);
    return (0);
}
