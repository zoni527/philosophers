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

unsigned int	time_diff_ms(const struct timeval *t1, const struct timeval *t2)
{
	int	seconds;
	int	microseconds;
	int	milliseconds;

	seconds = (int)(t2->tv_sec - t1->tv_sec);
	microseconds = (int)(t2->tv_usec - t1->tv_usec);
	milliseconds = (unsigned int)(seconds * 1000 + microseconds / 1000);
	return (milliseconds);
}

unsigned int	ms_from_start(const struct timeval *start_time)
{
	struct timeval	time;
	unsigned int	ms;

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

int	wait_and_try_not_to_die(t_philo *philo, unsigned long ms)
{
	struct timeval	t1;
	struct timeval	t2;
	unsigned long	ms_waited;

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
	data->start_time.tv_sec = time.tv_sec + 2;
	data->start_time.tv_usec = 0;
	i = 0;
	while (i < data->n_philos)
		data->philos[i++].last_eaten = data->start_time;
}
