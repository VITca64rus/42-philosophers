/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 13:40:30 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/12 15:48:27 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philo.h"

pthread_mutex_t	*ft_create_forks(int count)
{
	pthread_mutex_t	*forks;
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

static int	ft_get_num_fork(t_data *data, int i)
{
	if (i + 1 < data->count)
		return (i + 1);
	else
		return (0);
}

static int	ft_get_count_eat(int argc, char **argv)
{
	if (argc == 6)
		return (ft_atoi(argv[5]));
	else
		return (0);
}

t_philos	*ft_create_philosophers(t_data *data, char **argv, int argc, \
									pthread_mutex_t *entry_point)
{
	t_philos	*philosophers;
	int			i;

	philosophers = (t_philos *)malloc(sizeof(t_philos) * data->count);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < data->count)
	{
		philosophers[i].name = i + 1;
		philosophers[i].time_death = ft_atoi(argv[2]);
		philosophers[i].time_eat = ft_atoi(argv[3]);
		philosophers[i].time_sleap = ft_atoi(argv[4]);
		philosophers[i].last_eat = 0;
		philosophers[i].forks = data->forks;
		philosophers[i].stop = false;
		philosophers[i].time = 0;
		philosophers[i].left_fork = ft_get_num_fork(data, i);
		philosophers[i].right_fork = i;
		philosophers[i].now_count_eat = 0;
		philosophers[i].count_eat = ft_get_count_eat(argc, argv);
		philosophers[i].entry_point = entry_point;
		i++;
	}
	return (philosophers);
}
