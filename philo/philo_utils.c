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
	philo->forks_held[(philo->id + 1) % 2] = true;
	if (lock_and_report_activity(philo, M_TAKEN_FORK) == STOP_THE_PARTY)
	{
		pthread_mutex_unlock(philo->fork[(philo->id + 1) % 2]);
		return (STOP_THE_PARTY);
	}
	if (should_be_dead(philo))
		return (die_and_stop_the_party(philo));
	return (PARTY_ON);
}

int	take_second_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->fork[(philo->id + 2) % 2]);
	philo->forks_held[(philo->id + 2) % 2] = true;
	if (lock_and_report_activity(philo, M_TAKEN_FORK) == STOP_THE_PARTY)
	{
		drop_forks(philo);
		return (STOP_THE_PARTY);
	}
	if (should_be_dead(philo))
		return (die_and_stop_the_party(philo));
	return (PARTY_ON);
}

void	drop_forks(t_philo *philo)
{
	if (philo->forks_held[LEFT])
	{
		pthread_mutex_unlock(philo->fork[LEFT]);
		philo->forks_held[LEFT] = false;
	}
	if (philo->forks_held[RIGHT])
	{
		pthread_mutex_unlock(philo->fork[RIGHT]);
		philo->forks_held[RIGHT] = false;
	}
}

bool	should_be_dead(t_philo *philo)
{
	struct timeval	time;
	long			ms_from_last_eaten;

	gettimeofday(&time, NULL);
	ms_from_last_eaten = time_diff_ms(&philo->last_eaten, &time);
	if (ms_from_last_eaten > *philo->t_to_die)
		return (true);
	return (false);
}
