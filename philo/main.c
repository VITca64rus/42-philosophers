/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:38:28 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/12 17:43:40 by sazelda          ###   ########.fr       */
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
	free(threads);
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

	pthread_mutex_destroy(&entry_point);
	free(data->philosophers);
	free(data->forks);
	free(data);
	return (0);
}
