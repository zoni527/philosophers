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

int	main(int argc, char *argv[])
{
	static t_data	data;
	int				rval;

	if (argc < 5 || argc > 6)
		return (printendl_and_return(M_INPUT_EXAMPLE, E_INPUT));
	rval = input_check(argv);
	if (rval != 0)
		return (print_input_error(rval));
	if (setup_data(&data, argc, argv) == FAILURE)
		return (cleanup_and_return(&data, E_ALLOC));
	if (initialize_mutexes(&data) == FAILURE)
		return (cleanup_and_return(&data, E_MUTEX));
	setup_philosophers(&data);
	create_threads(&data);
	merge_threads(&data);
	destroy_mutexes(&data);
	cleanup_data(&data);
	return (EXIT_SUCCESS);
}

int	setup_data(t_data *data, int argc, char *argv[])
{
	struct timeval	time;

	data->n_philos = atou(argv[1]);
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
	data->threads = malloc(data->n_philos * sizeof(pthread_t));
	if (!data->philos || !data->forks || !data->threads)
		return (FAILURE);
	data->time_to_die = atou(argv[2]);
	data->time_to_eat = atou(argv[3]);
	data->time_to_sleep = atou(argv[4]);
	if (argc > 5)
		data->n_meals = atou(argv[5]);
	gettimeofday(&time, NULL);
	data->start_time.tv_sec = time.tv_sec + START_DELAY_S;
	data->start_time.tv_usec = 0;
	data->sim_active = true;
	return (SUCCESS);
}

int	initialize_mutexes(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			while (i--)
				pthread_mutex_destroy(&data->forks[i]);
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
		data->philos[i].alive = true;
		data->philos[i].last_eaten = data->start_time;
		data->philos[i].data = data;
		i++;
	}
}

int	create_threads(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_create(&data->threads[i], NULL, \
					 &philo_routine, &data->philos[i]))
		{
			while (i--)
				pthread_join(data->threads[i], NULL);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

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

int	print_input_error(int error)
{
	if (error == E_NAN)
		return (printendl_and_return(M_NAN, E_NAN));
	if (error == E_OVERFLOW)
		return (printendl_and_return(M_OVERFLOW, E_OVERFLOW));
	if (error == E_ZERO)
		return (printendl_and_return(M_ZERO, E_ZERO));
	return (error);
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

unsigned long	time_diff_ms(const struct timeval *t1, const struct timeval *t2)
{
	unsigned long	seconds;
	unsigned long	microseconds;
	unsigned long	milliseconds;

	seconds = t2->tv_sec - t1->tv_sec;
	microseconds = t2->tv_usec - t1->tv_usec;
	milliseconds = seconds * 1000 + microseconds / 1000;
	return (milliseconds);
}

bool	should_be_dead(t_philo *philo)
{
	struct timeval	time;
	unsigned long	ms_from_last_eaten;

	gettimeofday(&time, NULL);
	ms_from_last_eaten = time_diff_ms(&philo->last_eaten, &time);
	if (ms_from_last_eaten > philo->data->time_to_die)
		return (true);
	return (false);
}

void	philo_eat(t_philo *philo)
{
	struct timeval	time;
	unsigned long	ms_from_start_time;

	if (should_be_dead(philo))
	{
		pthread_mutex_lock(&philo->data->sim_lock);
		philo->data->sim_active = false;
		pthread_mutex_unlock(&philo->data->sim_lock);
		gettimeofday(&time, NULL);
		ms_from_start_time = time_diff_ms(&philo->data->start_time, &time);
		printf("%lu %d died\n", ms_from_start_time, philo->id);
		return ;
	}
	pthread_mutex_lock(philo->fork[LEFT]);
	gettimeofday(&time, NULL);
	ms_from_start_time = time_diff_ms(&philo->data->start_time, &time);
	printf("%lu %d has taken a fork\n", ms_from_start_time, philo->id);
	pthread_mutex_lock(philo->fork[RIGHT]);
	gettimeofday(&time, NULL);
	ms_from_start_time = time_diff_ms(&philo->data->start_time, &time);
	printf("%lu %d has taken a fork\n", ms_from_start_time, philo->id);
	gettimeofday(&time, NULL);
	printf("%lu %d is eating\n", ms_from_start_time, philo->id);
	philo->last_eaten = time;
	while (time_diff_ms(&philo->last_eaten, &time) < philo->data->time_to_die)
	{
		usleep(TICK);
		gettimeofday(&time, NULL);
	}
	pthread_mutex_unlock(philo->fork[LEFT]);
	pthread_mutex_unlock(philo->fork[RIGHT]);
}

void	philo_sleep(t_philo *philo)
{
	struct timeval	t1;
	struct timeval	t2;
	unsigned long	ms;

	if (should_be_dead(philo))
	{
		pthread_mutex_lock(&philo->data->sim_lock);
		philo->data->sim_active = false;
		pthread_mutex_unlock(&philo->data->sim_lock);
		gettimeofday(&t1, NULL);
		ms = time_diff_ms(&philo->data->start_time, &t1);
		printf("%lu %d died\n", ms, philo->id);
		return ;
	}
	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	ms = time_diff_ms(&t1, &t2);
	while (ms < philo->data->time_to_sleep)
	{
		usleep(TICK);
		gettimeofday(&t2, NULL);
		ms = time_diff_ms(&t1, &t2);
	}
}

unsigned long	ms_from_sim_start(const t_data *data)
{
	struct timeval	time;
	unsigned long	ms;

	gettimeofday(&time, NULL);
	ms = time_diff_ms(&data->start_time, &time);
	return (ms);
}

void	philo_think(t_philo *philo)
{
	struct timeval	time;

	if (should_be_dead(philo))
	{
		pthread_mutex_lock(&philo->data->sim_lock);
		philo->data->sim_active = false;
		pthread_mutex_unlock(&philo->data->sim_lock);
		gettimeofday(&time, NULL);
		printf("%lu %d died\n", ms_from_sim_start(philo->data), philo->id);
		return ;
	}
	gettimeofday(&time, NULL);
	printf("%lu %d is thinking\n", ms_from_sim_start(philo->data), philo->id);
}

void	wait_till_start(const t_philo *philo)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	while (time.tv_usec < philo->data->start_time.tv_usec \
		|| time.tv_sec < philo->data->start_time.tv_sec)
	{
		usleep(TICK);
		gettimeofday(&time, NULL);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	wait_till_start(philo);
	if (philo->id % 2 != 0)
		philo_think(philo);
	i = -1;
	while (philo->data->sim_active)
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void	destroy_mutexes(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

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

int	cleanup_and_return(t_data *data, int rval)
{
	cleanup_data(data);
	return rval;
}
