/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/01/24 17:01:59 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	gettimeofday(&tv, DST_NONE);
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

void	*live(void *args)
{
	t_table	*table;
	struct timeval tv;
	//long long time;

	table = (t_table *)args;
	//time = 0;
	if (table->philosopher.name % 2 == 0)
		usleep(500);
	int time_think = table->philosopher.time_eat - table->philosopher.time_sleap;
	long time = 0;
	long time1 = 0;
	long time_start_think = 0;
	int i = 0;
	while (1)
	{
		//pthread_mutex_lock(&entry_point);
		if (i == 0)
		{
			pthread_mutex_lock(&entry_point);
			gettimeofday(&tv, DST_NONE);
			time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			pthread_mutex_lock(&table->forks[table->philosopher.left_fork]);
			printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
			gettimeofday(&tv, DST_NONE);
			time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			pthread_mutex_lock(&table->forks[table->philosopher.right_fork]);
			printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
			table->philosopher.last_eat = time;
			pthread_mutex_unlock(&entry_point);
		}
		else
		{
			pthread_mutex_lock(&entry_point);
			gettimeofday(&tv, DST_NONE);
			time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			pthread_mutex_lock(&table->forks[table->philosopher.left_fork]);
			printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
			gettimeofday(&tv, DST_NONE);
			time = tv.tv_sec * 1000 + tv.tv_usec/1000;
			pthread_mutex_lock(&table->forks[table->philosopher.right_fork]);
			printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
			pthread_mutex_unlock(&entry_point);
		}
		// pthread_mutex_lock(&table->forks[table->philosopher.left_fork]);
		// printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
		// pthread_mutex_lock(&table->forks[table->philosopher.right_fork]);
		// printf("%lld %d has taken a fork\n",  time, table->philosopher.name);
		//usleep(500);
		//pthread_mutex_unlock(&entry_point);
		
		// gettimeofday(&tv, DST_NONE);
		// time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		printf("from eat %dms, for %d\n", time - table->philosopher.last_eat, table->philosopher.name);	
		if ((time - table->philosopher.last_eat) <= table->philosopher.time_death)
			printf("%lld %d is eating\n",  time, table->philosopher.name);
		else
		{
			printf("%lld %d died\n",  time, table->philosopher.name);
			exit(0);
		}
		usleep(table->philosopher.time_eat * 1000);
		table->philosopher.last_eat = time;
		time += table->philosopher.time_eat;
		pthread_mutex_unlock(&table->forks[table->philosopher.right_fork]);
		pthread_mutex_unlock(&table->forks[table->philosopher.left_fork]);
	
			
		printf("%lld %d is sleeping\n", time, table->philosopher.name);
		usleep(table->philosopher.time_sleap * 1000);
		time += table->philosopher.time_sleap;
		gettimeofday(&tv, DST_NONE);
		time_start_think = time;
		printf("%lld %d is thinking\n",  time, table->philosopher.name);
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t *forks;
	t_philos		*philosophers;
	t_table			*table;
	pthread_t 		*threads;
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
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		table[i].philosopher.last_eat = 0;
		pthread_create(&threads[i], NULL, live, &table[i]);
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return (0);
}