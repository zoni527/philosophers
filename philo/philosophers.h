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

# define LEFT	0
# define RIGHT	1

# define SUCCESS	0
# define FAILURE	1

# define M_INPUT_EXAMPLE	"How to use: ./philosophers <N of philosophers> " \
							"<T to die> <T to eat> <T to sleep> " \
							"(opt)<N of meals>"
# define M_NAN				"ERROR: only input positive numbers"
# define M_OVERFLOW			"ERROR: don't try to overflow inputs"
# define M_ZERO				"ERROR: use numbers larger than zero"

# define E_INPUT	1
# define E_ALLOC	2
# define E_NAN		3
# define E_OVERFLOW	4
# define E_ZERO		5

typedef struct s_data t_data;

typedef struct s_philo {
	int				id;
	pthread_mutex_t	*fork[2];
	bool			alive;
	struct timeval	last_eaten;
	bool			*sim_active;
	pthread_mutex_t	*sim_lock;
}	t_philo;

typedef struct s_data {
	bool			sim_active;
	pthread_mutex_t	sim_lock;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	struct timeval	start_time;
	unsigned int	n_meals;
	unsigned int	n_philos;
}	t_data;
#endif
