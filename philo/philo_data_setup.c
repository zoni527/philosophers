/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_data_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:24:12 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 17:26:22 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setup_data(t_data *data, int argc, char *argv[])
{
	struct timeval	time;

	data->n_philos = atou(argv[1]);
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
	data->threads = malloc(data->n_philos * sizeof(pthread_t));
	if (!data->philos || !data->forks || !data->threads)
		return (FAILURE);
	data->t_to_die = atou(argv[2]);
	data->t_to_eat = atou(argv[3]);
	data->t_to_sleep = atou(argv[4]);
	if (argc > 5)
		data->n_meals = atou(argv[5]);
	else
		data->n_meals = 0;
	gettimeofday(&time, NULL);
	data->start_time.tv_sec = 0;
	data->start_time.tv_usec = 0;
	data->sim_active = false;
	return (SUCCESS);
}

int	initialize_mutexes(t_data *data)
{
	unsigned int	i;

	if (pthread_mutex_init(&data->sim_lock, NULL))
		return (FAILURE);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			while (i--)
				pthread_mutex_destroy(&data->forks[i]);
			pthread_mutex_destroy(&data->sim_lock);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

void	setup_philosophers(t_data *data)
{
	unsigned int	i;
	struct timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	while (i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].fork[LEFT] = &data->forks[i];
		data->philos[i].fork[RIGHT] = &data->forks[(i + 1) % data->n_philos];
		data->philos[i].sim_lock = &data->sim_lock;
		data->philos[i].is_dead = false;
		data->philos[i].sim_active = &data->sim_active;
		data->philos[i].t_to_die = &data->t_to_die;
		data->philos[i].t_to_eat = &data->t_to_eat;
		data->philos[i].t_to_sleep = &data->t_to_sleep;
		data->philos[i].last_eaten = data->start_time;
		data->philos[i].start_time = &data->start_time;
		data->philos[i].n_meals = &data->n_meals;
		data->philos[i].meals_eaten = 0;
		i++;
	}
}

int	create_threads(t_data *data)
{
	unsigned int	i;

	pthread_mutex_lock(&data->sim_lock);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_create(&data->threads[i], NULL, \
					&philo_routine, &data->philos[i]))
		{
			while (i--)
				pthread_join(data->threads[i], NULL);
			pthread_mutex_unlock(&data->sim_lock);
			return (FAILURE);
		}
		i++;
	}
	set_start_times(data);
	data->sim_active = true;
	pthread_mutex_unlock(&data->sim_lock);
	return (SUCCESS);
}
