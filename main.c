/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/01/21 16:57:46 by sazelda          ###   ########.fr       */
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

	philosophers = (t_philos	*)malloc(sizeof(t_philos) * count);
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

	table = (t_table *)args;
	while (1)
	{
	pthread_mutex_lock(&entry_point);
	pthread_mutex_lock(&table->forks[table->philosopher.left_fork]);
	usleep(10);
	gettimeofday(&tv, DST_NONE);
	printf("%d %d has taken a fork\n",  tv.tv_sec * 1000 + tv.tv_usec, table->philosopher.name);
	pthread_mutex_lock(&table->forks[table->philosopher.right_fork]);
	usleep(10);
	gettimeofday(&tv, DST_NONE);
	printf("%d %d has taken a fork\n",  tv.tv_sec * 1000 + tv.tv_usec, table->philosopher.name);
	pthread_mutex_unlock(&entry_point);
	gettimeofday(&tv, DST_NONE);
	if (tv.tv_sec * 1000 + tv.tv_usec - table->philosopher.last_eat < table->philosopher.time_death)
		printf("%d %d is eating\n",  tv.tv_sec * 1000 + tv.tv_usec, table->philosopher.name);
	else
	{
		printf("%d %d died\n",  tv.tv_sec * 1000 + tv.tv_usec, table->philosopher.name);
		exit(0);
	}
	usleep(table->philosopher.time_eat);
	pthread_mutex_unlock(&table->forks[table->philosopher.left_fork]);
	pthread_mutex_unlock(&table->forks[table->philosopher.right_fork]);
	gettimeofday(&tv, DST_NONE);
	table->philosopher.last_eat = tv.tv_sec * 1000 + tv.tv_usec;
	printf("%d %d is sleaping\n",  tv.tv_sec * 1000 + tv.tv_usec, table->philosopher.name);
	usleep(200); // sleaping time FIX_me
	gettimeofday(&tv, DST_NONE);
	printf("%d %d is thinking\n",  tv.tv_sec * 1000 + tv.tv_usec, table->philosopher.name);
	usleep(200); // thinking time FIX_me
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
		gettimeofday(&tv, DST_NONE);
		table[i].philosopher.last_eat = tv.tv_sec * 1000 + tv.tv_usec;
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