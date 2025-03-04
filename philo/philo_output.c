/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:37:37 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:39:18 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	putendl_and_return(const char *str, int rval);

int	die_and_stop_the_party(t_philo *philo)
{
	pthread_mutex_lock(philo->sim_lock);
	if (*philo->sim_active == false)
	{
		pthread_mutex_unlock(philo->sim_lock);
		return (LEAVE_PARTY);
	}
	philo->is_dead = true;
	printf("[%ld] %u died\n", ms_from_time(philo->start_time), philo->id);
	*philo->sim_active = false;
	pthread_mutex_unlock(philo->sim_lock);
	return (LEAVE_PARTY);
}

int	lock_and_report_activity(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->sim_lock);
	if (*philo->sim_active == false)
	{
		pthread_mutex_unlock(philo->sim_lock);
		return (LEAVE_PARTY);
	}
	printf("[%ld] %u %s\n", ms_from_time(philo->start_time), philo->id, msg);
	pthread_mutex_unlock(philo->sim_lock);
	return (PARTY_ON);
}

int	print_input_error(int error)
{
	if (error == E_NAN)
		return (putendl_and_return(M_NAN, E_NAN));
	if (error == E_OVERFLOW)
		return (putendl_and_return(M_OVERFLOW, E_OVERFLOW));
	if (error == E_ZERO)
		return (putendl_and_return(M_ZERO, E_ZERO));
	return (error);
}

int	print_input_example(void)
{
	printf("Inputs (positive integers, time unit is ms):\n");
	printf("./philo <N of philos> <T to die> <T to eat> <T to sleep> ");
	printf("[<N of meals>]\n");
	return (E_INPUT);
}

static int	putendl_and_return(const char *str, int rval)
{
	printf("%s\n", str);
	return (rval);
}
