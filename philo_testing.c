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

#define THREADS	10

typedef struct s_threads {
	volatile int	count;
	int				n;
	pthread_mutex_t	mutex;
	pthread_mutex_t	print_lock;
}	t_threads;

void	*thread_routine_1(void *arg)
{
	t_threads	*data;
	int			id;
	int			counter;

	data = (t_threads *)arg;
	pthread_mutex_lock(&data->mutex);
	id = data->count;
	pthread_mutex_unlock(&data->mutex);
	counter = 1;
	while (counter--)
	{
		pthread_mutex_lock(&data->print_lock);
		printf("I am thread %d\n", id);
		pthread_mutex_unlock(&data->print_lock);
		if (usleep(1000000))
			exit(2);
	}
	for (int i = 0; i < 100; i++)
	{
		pthread_mutex_lock(&data->mutex);
		data->n++;
		pthread_mutex_unlock(&data->mutex);
	}
	pthread_mutex_lock(&data->print_lock);
	printf("Thread routine 1 done\n");
	pthread_mutex_unlock(&data->print_lock);
	return (NULL);
}

void	*prime_routine(void *arg)
{
	int	*i;

	i = (int *)arg;
	printf("Prime at received address: %d\n", *i);
	return (NULL);
}

void	*sum_routine(void *arg)
{
	
}

int	main(int argc, char *argv[])
{
	int						primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
	pthread_attr_t			attr;
	struct timeval			tv;
	pthread_t				th[THREADS];
	static t_threads		data;

	// obsolete: struct timezone *tz;
	if (gettimeofday(&tv, NULL))
		exit(1);
	printf("tv->tv_sec: %ld\n", tv.tv_sec);
	printf("tv->tv_usec: %ld\n", tv.tv_usec);
	printf("Calling usleep(1000000)\n");
	if (usleep(1000000))
		exit(2);
	printf("Hopefully here a second later\n");

	data.count = 0;
	printf("Calling pthread_create in a loop\n");
	for (int i = 0; i < THREADS; i++)
	{
		pthread_mutex_lock(&data.mutex);
		data.count++;
		pthread_mutex_unlock(&data.mutex);
		if (pthread_create(th + i, NULL, &prime_routine, primes + i))
			exit(1);
	}
	for (int i = 0; i < THREADS; i++)
		if (pthread_join(th[i], NULL))
			exit (2);
	printf("n: %d\n", data.n);
	return (0);
}
