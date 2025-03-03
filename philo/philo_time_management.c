/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_management.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:26:18 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:30:05 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_diff_ms(const struct timeval *t1, const struct timeval *t2)
{
	long	seconds;
	long	microseconds;
	long	milliseconds;

	seconds = t2->tv_sec - t1->tv_sec;
	microseconds = t2->tv_usec - t1->tv_usec;
	milliseconds = seconds * 1000 + microseconds / 1000;
	return (milliseconds);
}

long	ms_from_time(const struct timeval *start_time)
{
	struct timeval	time;
	long			ms;

	gettimeofday(&time, NULL);
	ms = time_diff_ms(start_time, &time);
	return (ms);
}

void	wait_till_start(const struct timeval *start_time)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	while (time.tv_usec < start_time->tv_usec \
		|| time.tv_sec < start_time->tv_sec)
	{
		usleep(TICK);
		gettimeofday(&time, NULL);
	}
}

int	wait_and_try_not_to_die(t_philo *philo, long ms)
{
	struct timeval	t1;
	struct timeval	t2;
	long			ms_waited;

	gettimeofday(&t1, NULL);
	ms_waited = 0;
	while (ms_waited < ms)
	{
		usleep(TICK);
		if (should_be_dead(philo))
			return (die_and_stop_the_party(philo));
		gettimeofday(&t2, NULL);
		ms_waited = time_diff_ms(&t1, &t2);
	}
	return (PARTY_ON);
}

void	set_start_times(t_data *data)
{
	struct timeval	time;
	unsigned int	i;

	gettimeofday(&time, NULL);
	data->start_time.tv_sec = time.tv_sec + \
		(time.tv_usec + SYNC_TIME) / 1000000;
	data->start_time.tv_usec = (time.tv_usec + SYNC_TIME) % 1000000;
	i = 0;
	while (i < data->n_philos)
		data->philos[i++].last_eaten = data->start_time;
}
