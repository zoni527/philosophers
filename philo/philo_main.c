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
	merge_threads(&data);
	destroy_mutexes(&data);
	cleanup_data(&data);
	return (EXIT_SUCCESS);
}
