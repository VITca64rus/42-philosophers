/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/07 16:50:20 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// NEW
#include "ft_philosophers.h"

pthread_mutex_t *ft_create_forks(int count)
{
	pthread_mutex_t *forks;
	int				i;

	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * count);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

t_philos	*ft_create_philosophers(int count, pthread_mutex_t *forks, char **argv, int argc)
{
	t_philos	*philosophers;
	int			i;
	int			j;

	philosophers = (t_philos *)malloc(sizeof(t_philos) * count);
	if (!philosophers)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		philosophers[i].name = i + 1;
		philosophers[i].time_death = ft_atoi(argv[2]);
		philosophers[i].time_eat = ft_atoi(argv[3]);
		philosophers[i].time_sleap = ft_atoi(argv[4]);
		philosophers[i].last_eat = 0;
		philosophers[i].forks = forks;
		philosophers[i].stop = false;
		philosophers[i].time = 0;
		if (j + 1 < count)
			philosophers[i].left_fork = j + 1;
		else
			philosophers[i].left_fork = 0;
		philosophers[i].right_fork = j;
		philosophers[i].now_count_eat = 0;
		if (argc == 6)
			philosophers[i].count_eat = ft_atoi(argv[5]);
		else
			philosophers[i].count_eat = 0;
		i++;
		j++;
	}
	return (philosophers);
}

void castom_usleep(long time)
{
	long i = 0;
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
long time_start = 0;
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
			printf("%lld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
			gettimeofday(&tv, NULL);
			philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			if (philo->stop)
				break ;
			pthread_mutex_lock(&entry_point);
			printf("%lld %d has taken a fork\n",  philo->time - time_start, philo->name);
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
			printf("%lld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
			gettimeofday(&tv, NULL);
			philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			if (philo->stop)
				break ;
			pthread_mutex_lock(&entry_point);
			printf("%lld %d has taken a fork\n",  philo->time - time_start, philo->name);
			pthread_mutex_unlock(&entry_point);
		}
			
			//if (philo->last_eat == 0)
			//	philo->last_eat = philo->time;
		
			if (philo->stop)
				break ;
			philo->last_eat = philo->time;
			pthread_mutex_lock(&entry_point);
			printf("%lld %d is eating\n",  philo->time - time_start, philo->name);	
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
		printf("%lld %d is sleeping\n", philo->time- time_start, philo->name);
		pthread_mutex_unlock(&entry_point);
		castom_usleep(philo->time_sleap);
		philo->time += philo->time_sleap;
		if (philo->stop)
			break ;
		pthread_mutex_lock(&entry_point);
		printf("%lld %d is thinking\n",  philo->time- time_start, philo->name);
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
	int count_not_want_eat;
	
	data = (t_data *)args;
	printf("MONI\n");
	int j = 0;
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
		printf("%lld %d died\n",  data->philosophers[0].time_death, data->philosophers[0].name);
	}
	else
	{
		if ((time - data->philosophers[i].last_eat > data->philosophers[i].time_death) && (data->philosophers[i].last_eat != 0) && (data->philosophers[i].time != 0) && (data->philosophers[i].last_eat != data->philosophers[i].time))
		{
			pthread_mutex_lock(&entry_point);
			//printf("%lld %d died\n",  data->philosophers[i].time - time_start, data->philosophers[i].name);
			data->philosophers[i].stop = true;
			//pthread_mutex_lock(&entry_point);
			j = 0;
			while (j < data->count)
			{
				data->philosophers[j].stop = true;
				j++;
			}
			printf("%lld %d died\n",  time - time_start, data->philosophers[i].name);
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
			//pthread_mutex_unlock(&entry_point);
			if (j >= data->count)
			{
				//pthread_mutex_lock(&entry_point);
				//data->philosophers[i].stop = true;
				//pthread_mutex_lock(&entry_point);
				j = 0;
				while (j < data->count)
				{
					data->philosophers[j].stop = true;
					j++;
				}
				
				//printf("%lld eat full\n",  time - time_start);
				pthread_mutex_unlock(&entry_point);
				break;
			//return(NULL);
			}
			pthread_mutex_unlock(&entry_point);
		}
		i++;
		if (i == data->count - 1)
			i = 0;
	}
	}
	i = 0;
	//printf("start clear forks\n");
	while (i < data->count)
	{
		//pthread_mutex_lock(&data->forks[i]);
		pthread_mutex_unlock(&data->forks[i]);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	//printf("start clear entry\n");
	//pthread_mutex_unlock(&entry_point);
	//pthread_mutex_destroy(&entry_point);
	//printf("finish MONI\n");
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
	struct timeval	tv;

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