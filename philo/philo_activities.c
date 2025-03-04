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
	if (there_has_been_an_error(philo))
		return (NULL);
	wait_till_start(philo->start_time);
	if (philo->id % 2 != 0)
		philo_think(philo);
	while (true)
	{
		if (philo_eat(philo) == LEAVE_PARTY)
			break ;
		if (philo_sleep(philo) == LEAVE_PARTY)
			break ;
		if (philo_think(philo) == LEAVE_PARTY)
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

	if (take_first_fork(philo) == LEAVE_PARTY)
		return (LEAVE_PARTY);
	if (take_second_fork(philo) == LEAVE_PARTY)
		return (LEAVE_PARTY);
	gettimeofday(&time, NULL);
	pthread_mutex_lock(philo->sim_lock);
	philo->last_eaten = time;
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->sim_lock);
	if (lock_and_report_activity(philo, M_EATING) == LEAVE_PARTY)
		return (LEAVE_PARTY);
	if (wait_and_try_not_to_die(philo, *philo->eat_t) == LEAVE_PARTY)
		return (LEAVE_PARTY);
	if (*philo->n_meals != 0 && philo->meals_eaten >= *philo->n_meals)
		return (LEAVE_PARTY);
	drop_forks(philo);
	return (PARTY_ON);
}

static int	philo_sleep(t_philo *philo)
{
	if (lock_and_report_activity(philo, M_SLEEPING) == LEAVE_PARTY)
		return (LEAVE_PARTY);
	if (wait_and_try_not_to_die(philo, *philo->sleep_t) == LEAVE_PARTY)
		return (LEAVE_PARTY);
	return (PARTY_ON);
}

static int	philo_think(t_philo *p)
{
	long	available_thinking_time;
	long	thinking_time;

	if (lock_and_report_activity(p, "is thinking") == LEAVE_PARTY)
		return (LEAVE_PARTY);
	if (p->meals_eaten == 0)
	{
		wait_and_try_not_to_die(p, *p->eat_t / 2);
		return (PARTY_ON);
	}
	available_thinking_time = *p->die_t - *p->eat_t - *p->sleep_t;
	if (available_thinking_time <= 20)
		thinking_time = 1;
	else
		thinking_time = available_thinking_time / 2 + 1;
	wait_and_try_not_to_die(p, thinking_time);
	return (PARTY_ON);
}
