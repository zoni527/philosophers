/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_activities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:10:07 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:26:06 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_eat(t_philo *philo);
static int	philo_sleep(t_philo *philo);
static int	philo_think(t_philo *philo);

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!party_still_going(philo))
		usleep(TICK);
	wait_till_start(philo->start_time);
	if (philo->id % 2 != 0)
		philo_think(philo);
	while (true)
	{
		if (philo_eat(philo) == STOP_THE_PARTY)
			break ;
		if (philo_sleep(philo) == STOP_THE_PARTY)
			break ;
		if (philo_think(philo) == STOP_THE_PARTY)
			break ;
	}
	drop_forks(philo);
	return (NULL);
}

bool	party_still_going(t_philo *philo)
{
	bool	rval;

	pthread_mutex_lock(philo->sim_lock);
	rval = *philo->sim_active;
	pthread_mutex_unlock(philo->sim_lock);
	return (rval);
}

static int	philo_eat(t_philo *philo)
{
	struct timeval	time;

	if (take_first_fork(philo) == STOP_THE_PARTY)
		return (STOP_THE_PARTY);
	if (take_second_fork(philo) == STOP_THE_PARTY)
		return (STOP_THE_PARTY);
	gettimeofday(&time, NULL);
	pthread_mutex_lock(philo->sim_lock);
	philo->last_eaten = time;
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->sim_lock);
	if (lock_and_report_activity(philo, M_EATING) == STOP_THE_PARTY)
		return (STOP_THE_PARTY);
	if (wait_and_try_not_to_die(philo, *philo->t_to_eat) == STOP_THE_PARTY)
	{
		drop_forks(philo);
		return (STOP_THE_PARTY);
	}
	if (*philo->n_meals != 0 && philo->meals_eaten >= *philo->n_meals)
	{
		drop_forks(philo);
		return (STOP_THE_PARTY);
	}
	drop_forks(philo);
	return (PARTY_ON);
}

static int	philo_sleep(t_philo *philo)
{
	if (lock_and_report_activity(philo, M_SLEEPING) == STOP_THE_PARTY)
		return (STOP_THE_PARTY);
	if (wait_and_try_not_to_die(philo, *philo->t_to_sleep) == STOP_THE_PARTY)
		return (STOP_THE_PARTY);
	return (PARTY_ON);
}

static int	philo_think(t_philo *philo)
{
	if (lock_and_report_activity(philo, "is thinking") == STOP_THE_PARTY)
		return (STOP_THE_PARTY);
	if (philo->meals_eaten == 0)
	{
		wait_and_try_not_to_die(philo, *philo->t_to_eat / 2);
		return (PARTY_ON);
	}
	if (ms_from_time(&philo->last_eaten) > *philo->t_to_die / 2)
		usleep(TICK);
	else
		wait_and_try_not_to_die(philo, *philo->t_to_eat / 2 + 2);
	return (PARTY_ON);
}
