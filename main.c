/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/03 15:57:33 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// NEW
#include "ft_philosophers.h"
pthread_t 		*threads;

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

t_philos	*ft_create_philosophers(int count, pthread_mutex_t *forks, char **argv)
{
	t_philos	*philosophers;
	int			i;
	int			j;
	struct timeval tv;

	philosophers = (t_philos *)malloc(sizeof(t_philos) * count);
	if (!philosophers)
		return (NULL);
	i = 0;
	j = 0;
	gettimeofday(&tv, NULL);
	while (i < count)
	{
		philosophers[i].name = i + 1;
		philosophers[i].time_death = ft_atoi(argv[2]);
		philosophers[i].time_eat = ft_atoi(argv[3]);
		philosophers[i].time_sleap = ft_atoi(argv[4]);
		philosophers[i].last_eat = tv.tv_sec * 1000 + tv.tv_usec;
		if (j + 1 < count)
			philosophers[i].left_fork = j + 1;
		else
			philosophers[i].left_fork = 0;
		philosophers[i].right_fork = j;
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
		gettimeofday(&tv, NULL);
}

void	*live(void *args)
{
	t_table	*table;
	struct timeval tv;
	//long long time;

	table = (t_table *)args;
	//time = 0;
	if (table->philosopher.name % 2 == 0)
		usleep(500);
	int time_think = 0;
	long time = 0;
	long time1 = 0;
	long time_start_think = 0;
	int i = 0;
	while (!table->stop)
	{
	
			if (table->stop)
			break ;
			if (table->philosopher.right_fork > table->philosopher.left_fork)
			{
				pthread_mutex_lock(&table->forks[table->philosopher.left_fork]);
				gettimeofday(&tv, NULL);
				time = tv.tv_sec * 1000 + tv.tv_usec/1000;
				pthread_mutex_lock(&entry_point);
				printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
				pthread_mutex_unlock(&entry_point);
			
				pthread_mutex_lock(&table->forks[table->philosopher.right_fork]);
				gettimeofday(&tv, NULL);
				time = tv.tv_sec * 1000 + tv.tv_usec/1000;
				pthread_mutex_lock(&entry_point);
				printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
				pthread_mutex_unlock(&entry_point);
			}
			else
			{
				pthread_mutex_lock(&table->forks[table->philosopher.right_fork]);
				gettimeofday(&tv, NULL);
				time = tv.tv_sec * 1000 + tv.tv_usec/1000;
				pthread_mutex_lock(&entry_point);
				printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
				pthread_mutex_unlock(&entry_point);
			
				pthread_mutex_lock(&table->forks[table->philosopher.left_fork]);
				gettimeofday(&tv, NULL);
				time = tv.tv_sec * 1000 + tv.tv_usec/1000;
				pthread_mutex_lock(&entry_point);
				printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
				pthread_mutex_unlock(&entry_point);
			}
		//	pthread_mutex_unlock(&entry_point);
			
			if (table->philosopher.last_eat == 0)
				table->philosopher.last_eat = time;
			//time_think = time - time1;
		
		
		if (table->stop)
			break ;
		printf("from eat %dms, for %d\n", time - table->philosopher.last_eat, table->philosopher.name);	
		if ((time - table->philosopher.last_eat) <= table->philosopher.time_death)
		{
			pthread_mutex_lock(&entry_point);
			printf("%lld %d is eating\n",  time, table->philosopher.name);	
			pthread_mutex_unlock(&entry_point);
		}
		else
		{
			pthread_mutex_lock(&entry_point);
			printf("%lld %d died\n",  time, table->philosopher.name);
			table->philosopher.name = 0;
			return (NULL);
		}
		if (table->stop)
			break ;
		castom_usleep(table->philosopher.time_eat);

			if (table->philosopher.right_fork > table->philosopher.left_fork)
			{
				pthread_mutex_unlock(&table->forks[table->philosopher.right_fork]);
				pthread_mutex_unlock(&table->forks[table->philosopher.left_fork]);
			}
			else
			{
				pthread_mutex_unlock(&table->forks[table->philosopher.left_fork]);
				pthread_mutex_unlock(&table->forks[table->philosopher.right_fork]);
			}
		if (table->stop)
			break ;
		table->philosopher.last_eat = time;
		time += table->philosopher.time_eat;
			pthread_mutex_lock(&entry_point);
		printf("%lld %d is sleeping\n", time, table->philosopher.name);
		pthread_mutex_unlock(&entry_point);
		//usleep(table->philosopher.time_sleap * 1000);
		castom_usleep(table->philosopher.time_sleap);
		time += table->philosopher.time_sleap;
		if (table->stop)
			break ;
		pthread_mutex_lock(&entry_point);
		printf("%lld %d is thinking\n",  time, table->philosopher.name);
		pthread_mutex_unlock(&entry_point);
		i++;
	}
	return (NULL);
}
pthread_mutex_t *forks;
void	*moni(void *args)
{
	t_table			*table;
	int i = 0;
	struct timeval tv;
	long time;
	
	table = (t_table *)args;
	printf("MONI\n");
	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec/1000;
	while (!table[i].stop)
	{
		gettimeofday(&tv, NULL);
		time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		//printf("find %ld > %ld\n", time - table[i].philosopher.last_eat, table[i].philosopher.time_death);
		if (time - table[i].philosopher.last_eat > table[i].philosopher.time_death && table[i].philosopher.last_eat != 0)
		{
			table[i].stop = true;
			if (table[i].stop)
				break;
		}
		i++;
		if (i == 4)
			i = 0;
	}
	printf("%lld %d died\n",  time, table->philosopher.name);
	pthread_mutex_destroy(&entry_point);
	while (i < 4)
	{
		table[i].stop = true;
		i++;
	}
	i = 0;
	while (i < 4)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	return(NULL);
}

int	main(int argc, char **argv)
{
	
	t_philos		*philosophers;
	t_table			*table;
	
	pthread_t 		monitor;
	int				i;
	struct timeval tv;

	if (argc != 5)
		return (0);
	forks = ft_create_forks(ft_atoi(argv[1]));
	philosophers = ft_create_philosophers(ft_atoi(argv[1]), forks, argv);
	
	table = (t_table *)malloc(sizeof(t_table) * ft_atoi(argv[1]));
	threads = (pthread_t *)malloc(sizeof(pthread_t) * ft_atoi(argv[1])); //CHECK FIX_ME
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		table[i].philosopher = philosophers[i];
		table[i].forks = forks;
		table[i].stop = false;
		i++;
	}
	i = 0;
	pthread_create(&monitor, NULL, moni, table);
	//pthread_detach(monitor);
	while (i < ft_atoi(argv[1]))
	{
		table[i].philosopher.last_eat = 0;
		pthread_create(&threads[i], NULL, live, &table[i]);
		
		i++;
	}
	pthread_detach(monitor);
i = 0;
	//pthread_create(&monitor, NULL, moni, table);
	//pthread_detach(monitor);
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(threads[i],NULL);
		i++;
	}
	return (0);
}