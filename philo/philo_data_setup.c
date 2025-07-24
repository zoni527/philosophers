/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_data_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:24:12 by jvarila           #+#    #+#             */
/*   Updated: 2025/03/04 15:42:56 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setup_data(t_data *data, int argc, char *argv[])
{
	struct timeval	time;

	data->n_philos = atou(argv[1]);
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	data->forks = malloc(data->n_philos * sizeof(t_fork));
	data->threads = malloc(data->n_philos * sizeof(pthread_t));
	if (!data->philos || !data->forks || !data->threads)
		return (FAILURE);
	data->die_t = atou(argv[2]);
	data->eat_t = atou(argv[3]);
	data->sleep_t = atou(argv[4]);
	if (argc > 5)
		data->n_meals = atou(argv[5]);
	else
		data->n_meals = 0;
	gettimeofday(&time, NULL);
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
		if (pthread_mutex_init(&data->forks[i].lock, NULL))
		{
			while (i--)
				pthread_mutex_destroy(&data->forks[i].lock);
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

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].fork[LEFT] = &data->forks[i];
		data->philos[i].fork[LEFT]->on_table = true;
		data->philos[i].fork[RIGHT] = &data->forks[(i + 1) % data->n_philos];
		data->philos[i].fork[RIGHT]->on_table = true;
		data->philos[i].sim_lock = &data->sim_lock;
		data->philos[i].is_dead = false;
		data->philos[i].error = &data->error;
		data->philos[i].sim_active = &data->sim_active;
		data->philos[i].die_t = &data->die_t;
		data->philos[i].eat_t = &data->eat_t;
		data->philos[i].sleep_t = &data->sleep_t;
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
					&philo_routine, &data->philos[i]) != 0)
		{
			data->error = true;
			data->sim_active = true;
			pthread_mutex_unlock(&data->sim_lock);
			while (i--)
				pthread_join(data->threads[i], NULL);
			printf("%s\n", M_THREAD);
			return (FAILURE);
		}
		i++;
	}
	set_start_times(data);
	data->sim_active = true;
	pthread_mutex_unlock(&data->sim_lock);
	return (SUCCESS);
}

bool	there_has_been_an_error(t_philo *philo)
{
	bool	rval;

	pthread_mutex_lock(philo->sim_lock);
	rval = *philo->error;
	pthread_mutex_unlock(philo->sim_lock);
	return (rval);
}
