/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:39:48 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:40:40 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_fork(t_philo *philo, unsigned int index);

int	take_first_fork(t_philo *philo)
{
	return (take_fork(philo, (philo->id + 1) % 2));
}

int	take_second_fork(t_philo *philo)
{
	return (take_fork(philo, (philo->id + 2) % 2));
}

static int	take_fork(t_philo *philo, unsigned int index)
{
	bool	no_fork;

	no_fork = true;
	while (no_fork)
	{
		pthread_mutex_lock(&philo->fork[index]->lock);
		if (philo->fork[index]->on_table)
		{
			philo->fork[index]->on_table = false;
			no_fork = false;
		}
		pthread_mutex_unlock(&philo->fork[index]->lock);
		if (no_fork == false)
			break ;
		usleep(TICK);
		if (should_be_dead(philo))
			return (die_and_stop_the_party(philo));
		if (party_still_going(philo) == false)
			return (LEAVE_PARTY);
	}
	return (PARTY_ON);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork[LEFT]->lock);
	philo->fork[LEFT]->on_table = true;
	pthread_mutex_unlock(&philo->fork[LEFT]->lock);
	pthread_mutex_lock(&philo->fork[RIGHT]->lock);
	philo->fork[RIGHT]->on_table = true;
	pthread_mutex_unlock(&philo->fork[RIGHT]->lock);
}

bool	should_be_dead(t_philo *philo)
{
	struct timeval	time;
	long			ms_from_last_eaten;
	bool			dead_should_be;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(philo->sim_lock);
	ms_from_last_eaten = time_diff_ms(&philo->last_eaten, &time);
	if (ms_from_last_eaten > *philo->die_t)
		dead_should_be = true;
	else
		dead_should_be = false;
	pthread_mutex_unlock(philo->sim_lock);
	return (dead_should_be);
}
