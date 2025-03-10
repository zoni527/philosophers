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
# define LEAVE_PARTY	1

// Time unit is microseconds
# define TICK			300
# define SYNC_TIME		5000

# define E_INPUT		1
# define E_ALLOC		2
# define E_NAN			3
# define E_OVERFLOW		4
# define E_ZERO			5
# define E_MUTEX		6
# define E_THREAD		7

# define M_NAN			"ERROR: only input positive numbers"
# define M_OVERFLOW		"ERROR: don't try to overflow inputs"
# define M_ZERO			"ERROR: use numbers larger than zero"
# define M_THREAD		"ERROR: thread creation failed, aborting"

# define M_TAKEN_FORK	"has taken a fork"
# define M_EATING		"is eating"
# define M_SLEEPING		"is sleeping"
# define M_THINKING		"is thinking"

typedef struct s_fork {
	pthread_mutex_t	lock;
	bool			on_table;
}	t_fork;

typedef struct s_philo {
	unsigned int	id;
	t_fork			*fork[2];
	pthread_mutex_t	*sim_lock;
	bool			is_dead;
	bool			*sim_active;
	bool			*error;
	unsigned int	*die_t;
	unsigned int	*eat_t;
	unsigned int	*sleep_t;
	unsigned int	*n_meals;
	unsigned int	meals_eaten;
	struct timeval	last_eaten;
	struct timeval	*start_time;
}	t_philo;

typedef struct s_data {
	bool			sim_active;
	bool			error;
	pthread_mutex_t	sim_lock;
	t_philo			*philos;
	t_fork			*forks;
	pthread_t		*threads;
	unsigned int	n_philos;
	unsigned int	die_t;
	unsigned int	eat_t;
	unsigned int	sleep_t;
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
bool			there_has_been_an_error(t_philo *philo);
/* philo_activities.c --------------------------------------------------------*/
void			*philo_routine(void *arg);
bool			party_still_going(t_philo *philo);
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
void			drop_forks(t_philo *philo);
bool			should_be_dead(t_philo *philo);
/*----------------------------------------------------------------------------*/
#endif
