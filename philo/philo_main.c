/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:34:17 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:40:55 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	observe_the_party(t_data *data);
static bool	philo_has_eaten_enough(t_philo *philo);

int	main(int argc, char *argv[])
{
	static t_data	data;
	int				inputs_ok;

	if (argc < 5 || argc > 6)
		return (print_input_example());
	inputs_ok = input_check(argv);
	if (inputs_ok != SUCCESS)
		return (print_input_error(inputs_ok));
	if (setup_data(&data, argc, argv) == FAILURE)
		return (cleanup_and_return(&data, E_ALLOC));
	if (initialize_mutexes(&data) == FAILURE)
		return (cleanup_and_return(&data, E_MUTEX));
	setup_philosophers(&data);
	if (create_threads(&data) == FAILURE)
	{
		destroy_mutexes(&data);
		return (cleanup_and_return(&data, E_THREAD));
	}
	observe_the_party(&data);
	merge_threads(&data);
	destroy_mutexes(&data);
	cleanup_data(&data);
	return (EXIT_SUCCESS);
}

static void	observe_the_party(t_data *data)
{
	unsigned int	i;
	unsigned int	full_philos;

	wait_till_start(&data->start_time);
	while (party_still_going(&data->philos[0]))
	{
		full_philos = 0;
		i = 0;
		while (i < data->n_philos)
		{
			if (should_be_dead(&data->philos[i]) \
				&& !philo_has_eaten_enough(&data->philos[i]))
			{
				die_and_stop_the_party(&data->philos[i]);
				break ;
			}
			if (philo_has_eaten_enough(&data->philos[i]))
				full_philos++;
			i++;
		}
		if (full_philos == data->n_philos)
			break ;
	}
}

static bool	philo_has_eaten_enough(t_philo *philo)
{
	bool	cant_eat_more;

	pthread_mutex_lock(philo->sim_lock);
	if (philo->meals_eaten >= *philo->n_meals && *philo->n_meals != 0)
		cant_eat_more = true;
	else
		cant_eat_more = false;
	pthread_mutex_unlock(philo->sim_lock);
	return (cant_eat_more);
}
