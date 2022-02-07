#include "ft_philo.h"

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

t_philos	*ft_create_philosophers(int count, pthread_mutex_t *forks, char **argv, int argc, pthread_mutex_t *entry_point)
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
		philosophers[i].entry_point = entry_point;
		i++;
		j++;
	}
	return (philosophers);
}