#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 200

typedef struct s_fork
{
    int id;
    pthread_mutex_t mutex;
} t_fork;

typedef struct s_data t_data;

typedef struct s_philosopher
{
    int id;
    int meals_eaten;
    int ate_enough;
    long long last_meal_time;
    pthread_mutex_t philo_mutex;
    t_fork *left_fork;
    t_fork *right_fork;
    t_data *data;
} t_philosopher;

struct s_data
{
    int num_philosophers;
    long long time_to_die;
    long long time_to_eat;
    long long time_to_sleep;
    int num_meals;
    int simulation_end;
    long long start_time;
    pthread_mutex_t output_mutex;
    pthread_mutex_t start_mutex;
    t_philosopher philosophers[BUFFER_SIZE];
    t_fork forks[BUFFER_SIZE];
};

long long ft_atoll(const char *str);
int is_valid_number(const char *str);
void set_simulation_end(t_data *data, int value);
int get_simulation_end(t_data *data);
void set_last_meal_time(t_philosopher *philo, long long time);
long long get_last_meal_time(t_philosopher *philo);
void set_meals_eaten(t_philosopher *philo, int meals);
int get_meals_eaten(t_philosopher *philo);
void set_ate_enough(t_philosopher *philo, int value);
int get_ate_enough(t_philosopher *philo);
long long get_time_in_ms(void);
void custom_usleep(long long time_in_ms, t_data *data);
void print_message(t_philosopher *philo, char *message);
void philosopher_eat(t_philosopher *philo);
void *philosopher_life(void *philosopher);
int parse_arguments(int argc, char **argv, t_data *data);
int initialize_simulation(t_data *data);
void cleanup(t_data *data);
void *single_philosopher(void *philosopher);

#endif
