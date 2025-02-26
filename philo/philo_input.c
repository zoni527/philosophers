/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:31:33 by jvarila           #+#    #+#             */
/*   Updated: 2025/02/26 16:31:34 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_positive_number(const char *str);
static bool	overflows(const char *str);

int	input_check(char *argv[])
{
	argv++;
	while (*argv)
	{
		if (!is_positive_number(*argv))
			return (E_NAN);
		if (overflows(*argv))
			return (E_OVERFLOW);
		if (atou(*argv) == 0)
			return (E_ZERO);
		argv++;
	}
	return (0);
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

static bool	is_positive_number(const char *str)
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

static bool	overflows(const char *str)
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
