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

int	take_first_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->fork[(philo->id + 1) % 2]);
	if (lock_and_report_activity(philo, M_TAKEN_FORK) == STOP_THE_PARTY)
	{
		pthread_mutex_unlock(philo->fork[(philo->id + 1) % 2]);
		return (STOP_THE_PARTY);
	}
	if (should_be_dead(philo))
	{
		pthread_mutex_unlock(philo->fork[(philo->id + 1) % 2]);
		return (die_and_stop_the_party(philo));
	}
	return (PARTY_ON);
}

int	take_second_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->fork[(philo->id + 2) % 2]);
	if (lock_and_report_activity(philo, M_TAKEN_FORK) == STOP_THE_PARTY)
	{
		pthread_mutex_unlock(philo->fork[LEFT]);
		pthread_mutex_unlock(philo->fork[RIGHT]);
		return (STOP_THE_PARTY);
	}
	if (should_be_dead(philo))
	{
		pthread_mutex_unlock(philo->fork[LEFT]);
		pthread_mutex_unlock(philo->fork[RIGHT]);
		return (die_and_stop_the_party(philo));
	}
	return (PARTY_ON);
}

int	starve(t_philo *philo)
{
	while (!should_be_dead(philo))
		usleep(TICK);
	die_and_stop_the_party(philo);
	return (STOP_THE_PARTY);
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork[LEFT]);
	pthread_mutex_unlock(philo->fork[RIGHT]);
}

bool	should_be_dead(t_philo *philo)
{
	struct timeval	time;
	unsigned long	ms_from_last_eaten;

	gettimeofday(&time, NULL);
	ms_from_last_eaten = time_diff_ms(&philo->last_eaten, &time);
	if (ms_from_last_eaten > *philo->t_to_die)
		return (true);
	return (false);
}
