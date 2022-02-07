/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/07 18:32:38 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philo.h"

// FIX_ME GLOBAL VARIABLES
pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;
long time_start = 0;

void castom_usleep(long time)
{
	struct timeval tv;
	long time1;
	gettimeofday(&tv, NULL);
	time1 = tv.tv_sec * 1000 + tv.tv_usec/1000;
	while (tv.tv_sec * 1000 + tv.tv_usec/1000 - time1 < time)
	{
		usleep(100);
		gettimeofday(&tv, NULL);
	}
}

void	*live(void *args)
{
	t_philos	*philo;
	struct timeval tv;
	int i = 0;

	philo = (t_philos *)args;
	gettimeofday(&tv, NULL);
	time_start = tv.tv_sec * 1000 + tv.tv_usec/1000;
	philo->last_eat = time_start;
	if (philo->name % 2 == 0)
		usleep(500);
	
	while (!philo->stop)
	{
		if (philo->right_fork > philo->left_fork)
		{
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
			gettimeofday(&tv, NULL);
			philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			if (philo->stop)
				break ;
			pthread_mutex_lock(&entry_point);
			printf("%ld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
			gettimeofday(&tv, NULL);
			philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			if (philo->stop)
				break ;
			pthread_mutex_lock(&entry_point);
			printf("%ld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
		}
		else
		{
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
			gettimeofday(&tv, NULL);
			philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			if (philo->stop)
				break ;
			pthread_mutex_lock(&entry_point);
			printf("%ld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
			gettimeofday(&tv, NULL);
			philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			if (philo->stop)
				break ;
			pthread_mutex_lock(&entry_point);
			printf("%ld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
		}
			if (philo->stop)
				break ;
			philo->last_eat = philo->time;
			pthread_mutex_lock(&entry_point);
			printf("%ld %d is eating\n",  philo->time - time_start, philo->name);	
			pthread_mutex_unlock(&entry_point);

		castom_usleep(philo->time_eat);
		philo->time += philo->time_eat;
			if (philo->right_fork > philo->left_fork)
			{
				pthread_mutex_unlock(&philo->forks[philo->right_fork]);
				pthread_mutex_unlock(&philo->forks[philo->left_fork]);
			}
			else
			{
				pthread_mutex_unlock(&philo->forks[philo->left_fork]);
				pthread_mutex_unlock(&philo->forks[philo->right_fork]);
			}
		philo->now_count_eat++;
		if (philo->stop)
			break ;
		pthread_mutex_lock(&entry_point);
		printf("%ld %d is sleeping\n", philo->time- time_start, philo->name);
		pthread_mutex_unlock(&entry_point);
		castom_usleep(philo->time_sleap);
		philo->time += philo->time_sleap;
		if (philo->stop)
			break ;
		pthread_mutex_lock(&entry_point);
		printf("%ld %d is thinking\n",  philo->time- time_start, philo->name);
		pthread_mutex_unlock(&entry_point);
		i++;
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
		pthread_mutex_lock(&entry_point);
		j = 0;
		while (j < data->count)
		{
			data->philosophers[j].stop = true;
			j++;
		}
		printf("%ld %d died\n",  data->philosophers[0].time_death, data->philosophers[0].name);
	}
	while (((1)) && (data->count != 1))
	{
		gettimeofday(&tv, NULL); 
		time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		
		if (data->count == 1)
		{
			pthread_mutex_lock(&entry_point);
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
				pthread_mutex_lock(&entry_point);
				data->philosophers[i].stop = true;
				j = 0;
				while (j < data->count)
				{
					data->philosophers[j].stop = true;
					j++;
				}
				printf("%ld %d died\n",  time - time_start, data->philosophers[i].name);
				pthread_mutex_unlock(&entry_point);
				break;
			}

			if (data->philosophers[i].count_eat <= data->philosophers[i].now_count_eat && data->philosophers[i].count_eat != 0)
			{
				pthread_mutex_lock(&entry_point);
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
					pthread_mutex_unlock(&entry_point);
					break;
				}
				pthread_mutex_unlock(&entry_point);
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

int	main(int argc, char **argv)
{
	
	t_philos		*philosophers;
	t_data			*data;
	pthread_mutex_t *forks;
	pthread_t 		*threads;
	pthread_t 		monitor;
	int				i;

	if (argc < 5 || argc > 6)
		return (0);
	forks = ft_create_forks(ft_atoi(argv[1]));
	philosophers = ft_create_philosophers(ft_atoi(argv[1]), forks, argv, argc);
	data = (t_data *)malloc(sizeof(data));
	data->count=ft_atoi(argv[1]);
	data->forks=forks;
	data->philosophers = philosophers;
	
	data = (t_data *)malloc(sizeof(t_data));
	data->count = ft_atoi(argv[1]);
	data->forks = forks;
	data->philosophers = philosophers;

	threads = (pthread_t *)malloc(sizeof(pthread_t) * ft_atoi(argv[1])); //CHECK FIX_ME

	pthread_create(&monitor, NULL, moni, data);
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_create(&threads[i], NULL, live, &philosophers[i]);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return (0);
}