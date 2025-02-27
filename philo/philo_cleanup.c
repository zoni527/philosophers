/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:06:41 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:32:03 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	merge_threads(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philos)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}

void	destroy_mutexes(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_destroy(&data->forks[i].lock);
		i++;
	}
	pthread_mutex_destroy(&data->sim_lock);
}

void	cleanup_data(t_data *data)
{
	free(data->philos);
	data->philos = NULL;
	free(data->forks);
	data->forks = NULL;
	free(data->threads);
	data->threads = NULL;
}

int	cleanup_and_return(t_data *data, int rval)
{
	cleanup_data(data);
	return (rval);
}
