/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_testing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 09:38:54 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/18 09:47:19 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void	thread_routine_1(void *arg)
{
	(void)arg;
	while (1)
	{
		printf("I am thread routine 1\n");
		if (usleep(1000000))
			exit(1);
	}
}

int	main(int argc, char *argv[])
{
	pthread_attr_t	attr;
	struct timeval	tv;
	pthread_t		thread;
	// obsolete: struct timezone *tz;
	if (gettimeofday(&tv, NULL))
		exit(1);
	printf("tv->tv_sec: %ld\n", tv.tv_sec);
	printf("tv->tv_usec: %ld\n", tv.tv_usec);
	printf("Calling usleep(1000000)\n");
	if (usleep(1000000))
		exit(2);
	printf("Hopefully here a second later\n");
	printf("Calling pthread_create()");
	if (pthread_create(&thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)
	return 0;
}
