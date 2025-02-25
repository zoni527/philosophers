/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:34:45 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/21 16:38:18 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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

# define ALIVE			0
# define DEAD			1

# define TICK			100

# define E_INPUT		1
# define E_ALLOC		2
# define E_NAN			3
# define E_OVERFLOW		4
# define E_ZERO			5
# define E_MUTEX		6
# define E_THREAD		7

# define M_INPUT_EXAMPLE	"Inputs (positive integers, time unit is ms):\n" \
							"./philo <N of philos> <T to die> <T to eat> " \
							"<T to sleep> (opt)<N of meals>"

# define M_NAN				"ERROR: only input positive numbers"
# define M_OVERFLOW			"ERROR: don't try to overflow inputs"
# define M_ZERO				"ERROR: use numbers larger than zero"

# define M_TAKEN_FORK	"has taken a fork"
# define M_EATING		"is eating"
# define M_SLEEPING		"is sleeping"
# define M_THINKING		"is thinking"

typedef struct s_data t_data;

typedef struct s_philo {
	int				id;
	pthread_mutex_t	*fork[2];
	struct timeval	last_eaten;
	t_data			*data;
}	t_philo;

typedef struct s_data {
	bool			sim_active;
	pthread_mutex_t	sim_lock;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	unsigned int	n_philos;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned int	n_meals;
	struct timeval	start_time;
}	t_data;

/*----------------------------------------------------------------------------*/
int				main(int argc, char *argv[]);
int				setup_data(t_data *data, int argc, char *argv[]);
int				initialize_mutexes(t_data *data);
void			setup_philosophers(t_data *data);
int				create_threads(t_data *data);
/*----------------------------------------------------------------------------*/
int				printendl_and_return(const char *str, int rval);
int				print_input_error(int error);
/*----------------------------------------------------------------------------*/
int				input_check(char *argv[]);
unsigned int	atou(const char *str);
bool			is_a_positive_number(const char *str);
bool			overflows(const char *str);
/*----------------------------------------------------------------------------*/
void			merge_threads(t_data *data);
void			destroy_mutexes(t_data *data);
void			cleanup_data(t_data *data);
int				cleanup_and_return(t_data *data, int rval);
/*----------------------------------------------------------------------------*/
void			*philo_routine(void *arg);
int				philo_eat(t_philo *philo);
int				philo_sleep(t_philo *philo);
int				philo_think(t_philo *philo);

bool			should_be_dead(t_philo *philo);

unsigned long	time_diff_ms(const struct timeval *t1, \
						   const struct timeval *t2);

unsigned long	ms_from_sim_start(const t_data *data);
void			wait_till_start(const t_philo *philo);

int				wait_and_try_not_to_die(t_philo *philo, unsigned long ms);
#endif
