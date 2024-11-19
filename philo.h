/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:30:00 by pouyaximac        #+#    #+#             */
/*   Updated: 2024/11/19 12:06:39 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* ************************************************************************** */
/*                                  Includes                                  */
/* ************************************************************************** */

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <string.h>

/* ************************************************************************** */
/*                                  Macros                                    */
/* ************************************************************************** */

# define BUFFER_SIZE 200

/* ************************************************************************** */
/*                                Structures                                  */
/* ************************************************************************** */

typedef struct s_data	t_data;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	int				ate_enough;
	long long		last_meal_time;
	pthread_mutex_t	philo_mutex;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_data			*data;
}	t_philosopher;

struct	s_data
{
	int				num_philosophers;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				num_meals;
	int				simulation_end;
	long long		start_time;
	pthread_mutex_t	output_mutex;
	pthread_mutex_t	simulation_end_mutex;
	pthread_mutex_t	start_mutex;
	t_philosopher	philosophers[BUFFER_SIZE];
	t_fork			forks[BUFFER_SIZE];
};

/* ************************************************************************** */
/*                            Function Prototypes                             */
/* ************************************************************************** */

/* utils.c */
long long	ft_atoll(const char *str);
int			is_valid_number(const char *str);
long long	get_time_in_ms(void);
void		custom_usleep(long long time_in_ms, t_data *data);

/* data_utils.c */
void		set_simulation_end(t_data *data, int value);
int			get_simulation_end(t_data *data);

/* philo_utils.c */
void		set_last_meal_time(t_philosopher *philo, long long time);
long long	get_last_meal_time(t_philosopher *philo);
void		set_meals_eaten(t_philosopher *philo, int meals);
int			get_meals_eaten(t_philosopher *philo);

/* philo_utils2.c */
void		set_ate_enough(t_philosopher *philo, int value);
int			get_ate_enough(t_philosopher *philo);

/* print.c */
void		print_message(t_philosopher *philo, char *message);

/* philosopher.c */
void		philosopher_eat(t_philosopher *philo);
void		*philosopher_life(void *philosopher);

/* init.c */
int			check_argc(int argc);
void		print_error(const char *message, size_t len);
int			initialize_mutexes(t_data *data);
void		initialize_forks(t_data *data);
void		initialize_philosophers(t_data *data);

/* cleanup.c */
void		cleanup(t_data *data);

/* monitor.c */
void		*monitor_philosophers(void *data_ptr);

/* thread_utils.c */
int			create_philosopher_threads(t_data *data, pthread_t *philosophers);
void		join_philosopher_threads(t_data *data, \
				pthread_t *philosophers, int num_threads);

/* single_philo.c */
void		*single_philosopher(void *philosopher);

/* main.c */
int			main(int argc, char **argv);

/* validation.c */
int			validate_arguments(int argc, char **argv);

#endif
