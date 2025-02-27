/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:34:45 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:32:35 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>

# define LEFT			0
# define RIGHT			1

# define SUCCESS		0
# define FAILURE		1

# define PARTY_ON		0
# define STOP_THE_PARTY	1

# define TICK			100

# define E_INPUT		1
# define E_ALLOC		2
# define E_NAN			3
# define E_OVERFLOW		4
# define E_ZERO			5
# define E_MUTEX		6
# define E_THREAD		7

# define M_NAN				"ERROR: only input positive numbers"
# define M_OVERFLOW			"ERROR: don't try to overflow inputs"
# define M_ZERO				"ERROR: use numbers larger than zero"

# define M_TAKEN_FORK	"has taken a fork"
# define M_EATING		"is eating"
# define M_SLEEPING		"is sleeping"
# define M_THINKING		"is thinking"

typedef struct s_data	t_data;

typedef struct s_philo {
	unsigned int	id;
	pthread_mutex_t	*fork[2];
	bool			forks_held[2];
	pthread_mutex_t	*sim_lock;
	bool			is_dead;
	bool			*sim_active;
	unsigned int	*t_to_die;
	unsigned int	*t_to_eat;
	unsigned int	*t_to_sleep;
	unsigned int	*n_meals;
	unsigned int	meals_eaten;
	struct timeval	last_eaten;
	struct timeval	*start_time;
}	t_philo;

typedef struct s_data {
	bool			sim_active;
	pthread_mutex_t	sim_lock;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	unsigned int	n_philos;
	unsigned int	t_to_die;
	unsigned int	t_to_eat;
	unsigned int	t_to_sleep;
	unsigned int	n_meals;
	struct timeval	start_time;
}	t_data;

/* philo_main.c --------------------------------------------------------------*/
int				main(int argc, char *argv[]);
/* philo_data_setup.c --------------------------------------------------------*/
int				setup_data(t_data *data, int argc, char *argv[]);
int				initialize_mutexes(t_data *data);
void			setup_philosophers(t_data *data);
int				create_threads(t_data *data);
/* philo_activities.c --------------------------------------------------------*/
void			*philo_routine(void *arg);
bool			sim_active(t_philo *philo);
/* philo_time_management.c ---------------------------------------------------*/
long			time_diff_ms(const struct timeval *t1, \
						const struct timeval *t2);
long			ms_from_time(const struct timeval *start_time);
void			wait_till_start(const struct timeval *start_time);
int				wait_and_try_not_to_die(t_philo *philo, long ms);
void			set_start_times(t_data *data);
/* philo_input.c -------------------------------------------------------------*/
int				input_check(char *argv[]);
unsigned int	atou(const char *str);
/* philo_output.c ------------------------------------------------------------*/
int				die_and_stop_the_party(t_philo *philo);
int				lock_and_report_activity(t_philo *philo, char *msg);
int				print_input_error(int error);
int				print_input_example(void);
/* philo_cleanup.c -----------------------------------------------------------*/
void			merge_threads(t_data *data);
void			destroy_mutexes(t_data *data);
void			cleanup_data(t_data *data);
int				cleanup_and_return(t_data *data, int rval);
/* philo_utils.c -------------------------------------------------------------*/
int				take_first_fork(t_philo *philo);
int				take_second_fork(t_philo *philo);
int				starve(t_philo *philo);
void			drop_forks(t_philo *philo);
bool			should_be_dead(t_philo *philo);
/*----------------------------------------------------------------------------*/
#endif
