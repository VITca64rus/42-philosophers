/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_moni.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:23:15 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/12 17:00:19 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philo.h"

static void	ft_one_phil(t_data *data)
{
	if (data->count == 1)
	{
		pthread_mutex_lock(data->philosophers[0].entry_point);
		data->philosophers[0].stop = true;
		printf("%ld %d died\n", data->philosophers[0].time_death, \
				data->philosophers[0].name);
		pthread_mutex_unlock(data->philosophers[0].entry_point);
	}
}

static int	ft_check_time(t_data *data, int i)
{
	int				j;
	struct timeval	tv;
	long			time;
	long			time_from_eat;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	time_from_eat = time - data->philosophers[i].last_eat;
	if ((time_from_eat > data->philosophers[i].time_death) \
		&& (data->philosophers[i].last_eat != data->philosophers[i].time))
	{
		pthread_mutex_lock(data->philosophers[i].entry_point);
		data->philosophers[i].stop = true;
		j = 0;
		while (j < data->count)
		{
			data->philosophers[j].stop = true;
			j++;
		}
		printf("%ld %d died\n", time - data->philosophers[i].time_start, \
				data->philosophers[i].name);
		pthread_mutex_unlock(data->philosophers[i].entry_point);
		return (0);
	}
	return (1);
}

static int	ft_get_count_well_fed(t_data *data)
{
	int	j;

	j = 0;
	while (j < data->count)
	{
		if (data->philosophers[j].count_eat <= \
			data->philosophers[j].now_count_eat)
			j++;
		else
			break ;
	}
	return (j);
}

static int	ft_check_count_eat(t_data *data, int i)
{
	int	j;

	if (data->philosophers[i].count_eat <= data->philosophers[i].now_count_eat \
		&& data->philosophers[i].count_eat != 0)
	{
		pthread_mutex_lock(data->philosophers[i].entry_point);
		j = ft_get_count_well_fed(data);
		if (j >= data->count)
		{
			j = 0;
			while (j < data->count)
			{
				data->philosophers[j].stop = true;
				j++;
			}
			pthread_mutex_unlock(data->philosophers[i].entry_point);
			return (0);
		}
		pthread_mutex_unlock(data->philosophers[i].entry_point);
	}
	return (1);
}

void	*moni(void *args)
{
	t_data			*data;
	int				i;

	data = (t_data *)args;
	usleep(100);
	ft_one_phil(data);
	i = 0;
	while (1 && (data->count != 1))
	{
		if (ft_check_time(data, i) == 0)
			break ;
		if (ft_check_count_eat(data, i) == 0)
			break ;
		i++;
		if (i == data->count - 1)
			i = 0;
	}
	i = 0;
	while (i < data->count)
	{
		pthread_mutex_unlock(&data->forks[i]);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	return (NULL);
}
