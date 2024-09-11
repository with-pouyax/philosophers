#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

typedef struct s_fork {
    pthread_mutex_t mutex;
} t_fork;

typedef struct s_philosopher {
    int             id;
    long long       last_meal_time;
    int             meals_eaten;
    int             ate_enough;
    t_fork          *left_fork;
    t_fork          *right_fork;
    struct s_data   *data;
} t_philosopher;

typedef struct s_data {
    int                 number_of_philosophers;
    long long           time_to_die;
    long long           time_to_eat;
    long long           time_to_sleep;
    int                 number_of_meals;
    int                 simulation_end;
    long long           start_time;
    pthread_mutex_t     print_mutex;
    pthread_mutex_t     simulation_mutex;
    t_philosopher       *philosophers;
    t_fork              *forks;
} t_data;

// Function declarations
void set_simulation_end(t_data *data, int value);
int get_simulation_end(t_data *data);
void set_last_meal_time(t_philosopher *philo, long long time);
long long get_last_meal_time(t_philosopher *philo);
void set_meals_eaten(t_philosopher *philo, int meals);
int get_meals_eaten(t_philosopher *philo);
void set_ate_enough(t_philosopher *philo, int value);
int get_ate_enough(t_philosopher *philo);

long long get_time_in_ms(t_data *data);
void custom_usleep(long long time_in_ms, t_data *data);
void print_message(t_philosopher *philo, char *message);
void philosopher_eat(t_philosopher *philo);
void *philosopher_life(void *philosopher);
void *death_monitor(void *data_void);
void *meal_monitor(void *data_void);

int parse_arguments(int argc, char **argv, t_data *data);
int initialize_simulation(t_data *data);
void cleanup(t_data *data, pthread_t *philosophers);
int is_valid_number(const char *str);
#endif
