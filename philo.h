#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

typedef struct s_philosopher {
	int					id;
	long long			last_meal_time;
	int					meals_eaten;
	pthread_mutex_t		left_fork;
	pthread_mutex_t		*right_fork;
	struct s_data		*data;
}	t_philosopher;

typedef struct s_data {
	int					number_of_philosophers;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					number_of_meals;
	int					simulation_end;
	long long			start_time;
	t_philosopher		*philosophers;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
}	t_data;

long long	get_time_in_ms(t_data *data);
void		precise_usleep(long long time_in_ms, t_data *data);
void		print_message(t_philosopher *philo, char *message);
void		philosopher_eat(t_philosopher *philo);
void		*philosopher_life(void *philosopher);
void		*death_monitor(void *data_void);
int			is_valid_number(const char *str);
int			parse_arguments(int argc, char **argv, t_data *data);
void		cleanup(t_data *data, pthread_t *philosophers);
int			initialize_simulation(t_data *data);

#endif
