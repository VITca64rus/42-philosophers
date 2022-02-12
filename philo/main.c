/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/12 14:29:47 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philo.h"

void	*live(void *args)
{
	t_philos		*philo;
	struct timeval	tv;

	philo = (t_philos *)args;
	gettimeofday(&tv, NULL);
	philo->time_start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	philo->last_eat = philo->time_start;
	if (philo->name % 2 == 0)
		usleep(500);
	while (!philo->stop)
	{
		if (ft_take_fork(philo) == 0)
			break ;
		if (ft_eat(philo) == 0)
			break ;
		if (ft_sleap(philo) == 0)
			break ;
		ft_custom_printf(philo, "is thinking");
	}
	return (NULL);
}

void	*moni(void *args)
{
	t_data			*data;
	int i = 0;
	struct timeval tv;
	long time;
	
	data = (t_data *)args;
	int j = 0;
	usleep(100);
	if (data->count == 1)
	{
		pthread_mutex_lock(data->philosophers[i].entry_point);
		j = 0;
		data->philosophers[j].stop = true;
		printf("%ld %d died\n",  data->philosophers[0].time_death, data->philosophers[0].name);
		pthread_mutex_unlock(data->philosophers[i].entry_point);
	}
	while (((1)) && (data->count != 1))
	{
		gettimeofday(&tv, NULL); 
		time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		
		if (data->count == 1)
		{
			pthread_mutex_lock(data->philosophers[i].entry_point);
			j = 0;
			while (j < data->count)
			{
				data->philosophers[j].stop = true;
				j++;
			}
			printf("%ld %d died\n",  data->philosophers[0].time_death, data->philosophers[0].name);
		}
		else
		{
			if ((time - data->philosophers[i].last_eat > data->philosophers[i].time_death) && (data->philosophers[i].last_eat != 0) && (data->philosophers[i].time != 0) && (data->philosophers[i].last_eat != data->philosophers[i].time))
			{
				pthread_mutex_lock(data->philosophers[i].entry_point);
				data->philosophers[i].stop = true;
				j = 0;
				while (j < data->count)
				{
					data->philosophers[j].stop = true;
					j++;
				}
				printf("%ld %d died\n",  time - data->philosophers[i].time_start, data->philosophers[i].name);
				pthread_mutex_unlock(data->philosophers[i].entry_point);
				break;
			}
			else if (data->philosophers[i].count_eat <= data->philosophers[i].now_count_eat && data->philosophers[i].count_eat != 0)
			{
				pthread_mutex_lock(data->philosophers[i].entry_point);
				j = 0;
				while (j < data->count)
				{
					if (data->philosophers[j].count_eat <= data->philosophers[j].now_count_eat)
						j++;
					else
						break;
				}
				if (j >= data->count)
				{
					j = 0;
					while (j < data->count)
					{
						data->philosophers[j].stop = true;
						j++;
					}
					pthread_mutex_unlock(data->philosophers[i].entry_point);
					break;
				}
				pthread_mutex_unlock(data->philosophers[i].entry_point);
			}
			i++;
			if (i == data->count - 1)
				i = 0;
		}
	}
	i = 0;
	while (i < data->count)
	{
		pthread_mutex_unlock(&data->forks[i]);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	return(NULL);
}

void	ft_threads(t_data	*data)
{
	pthread_t		*threads;
	pthread_t		monitor;
	int				i;

	threads = (pthread_t *)malloc(sizeof(pthread_t) * data->count);
	pthread_create(&monitor, NULL, moni, data);
	i = 0;
	while (i < data->count)
	{
		pthread_create(&threads[i], NULL, live, &data->philosophers[i]);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data->count)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data			*data;
	pthread_mutex_t	entry_point;

	pthread_mutex_init(&entry_point, NULL);
	if (argc < 5 || argc > 6 || ft_check(argc, argv) == 0)
	{
		printf("Error\n");
		return (0);
	}
	data = (t_data *)malloc(sizeof(t_data));
	data->count = ft_atoi(argv[1]);
	data->forks = ft_create_forks(data->count);
	data->philosophers = ft_create_philosophers(data, argv, argc, &entry_point);
	ft_threads(data);
	return (0);
}
