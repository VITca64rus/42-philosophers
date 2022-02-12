/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 13:35:03 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/12 13:40:19 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philo.h"

int	ft_custom_printf(t_philos *philo, char *s)
{
	pthread_mutex_lock(philo->entry_point);
	if (philo->stop)
	{
		pthread_mutex_unlock(philo->entry_point);
		return (0);
	}
	printf("%ld %d %s\n", philo->time - philo->time_start, philo->name, s);
	pthread_mutex_unlock(philo->entry_point);
	return (1);
}

void	castom_usleep(long time)
{
	struct timeval	tv;
	long			time1;

	gettimeofday(&tv, NULL);
	time1 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	while (tv.tv_sec * 1000 + tv.tv_usec / 1000 - time1 < time)
	{
		usleep(100);
		gettimeofday(&tv, NULL);
	}
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int	ft_check(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (ft_isdigit(argv[i][j]) == 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
