/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:34:17 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/21 11:53:16 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	printendl_and_return(const char *str, int rval)
{
	printf("%s\n", str);
	return (rval);
}

unsigned int	atou(const char *str)
{
	unsigned int	num;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+')
		str++;
	num = 0;
	while (*str >= '0' && *str <= '9')
		num = 10 * num + *(str++) - '0';
	return (num);
}

bool	is_a_positive_number(const char *str)
{
	if (!str || !*str)
		return (false);
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+')
		str++;
	if (*str < '0' || *str > '9')
		return (false);
	while (*str >= '0' && *str <= '9')
		str++;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str != '\0')
		return (false);
	return (true);
}

bool	overflows(const char *str)
{
	unsigned int	num;
	unsigned int	prev;

	num = 0;
	prev = num;
	while (*str < '0' || *str > '9')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		num = 10 * num + *(str++) - '0';
		if (num < prev)
			return (true);
		prev = num;
	}
	return (false);
}

int	input_check(char *argv[])
{
	argv++;
	while (*argv)
	{
		if (!is_a_positive_number(*argv))
			return (E_NAN);
		if (overflows(*argv))
			return (E_OVERFLOW);
		if (atou(*argv) == 0)
			return (E_ZERO);
		argv++;
	}
	return (0);
}

int	print_error(int	error)
{
	if (error == E_NAN)
		return (printendl_and_return(M_NAN, E_NAN));
	if (error == E_OVERFLOW)
		return (printendl_and_return(M_OVERFLOW, E_OVERFLOW));
	if (error == E_ZERO)
		return (printendl_and_return(M_ZERO, E_ZERO));
	return (error);
}

int	setup_data(t_data *data, int argc, char *argv[])
{
	data->n_philos = atou(argv[1]);
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
	if (!data->philos || !data->forks)
		return (FAILURE);
	data->time_to_die = atou(argv[2]);
	data->time_to_eat = atou(argv[3]);
	data->time_to_sleep = atou(argv[4]);
	if (argc > 5)
		data->n_meals = atou(argv[5]);
	return (SUCCESS);
}

void	cleanup_data(t_data *data)
{
	free(data->philos);
	data->philos = NULL;
	free(data->forks);
	data->forks = NULL;
}

void	setup_philosophers(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].fork[LEFT] = &data->forks[i];
		data->philos[i].fork[RIGHT] = &data->forks[(i + 1) % data->n_philos];
		data->philos[i].alive = true;
		data->philos[i].last_eaten.tv_sec = 0;
		data->philos[i].last_eaten.tv_usec = 0;
		data->philos[i].sim_active = &data->sim_active;
		data->philos[i].sim_lock = &data->sim_lock;
		i++;
	}
}

int	main(int argc, char *argv[])
{
	static t_data	data;
	int				rval;

	if (argc < 5 || argc > 6)
		return (printendl_and_return(M_INPUT_EXAMPLE, E_INPUT));
	rval = input_check(argv);
	if (rval != 0)
		return (print_error(rval));
	if (setup_data(&data, argc, argv) == FAILURE)
	{
		cleanup_data(&data);
		return (E_ALLOC);
	}
	setup_philosophers(&data);
	cleanup_data(&data);
	return (EXIT_SUCCESS);
}
