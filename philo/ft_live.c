#include "ft_philo.h"

void	ft_put_forks(t_philos *philo)
{
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
}

int	ft_eat(t_philos *philo)
{
	if (philo->stop)
		return (0);
	philo->last_eat = philo->time;
	pthread_mutex_lock(philo->entry_point);
	printf("%ld %d is eating\n",  philo->time - philo->time_start, philo->name);	
	pthread_mutex_unlock(philo->entry_point);
	castom_usleep(philo->time_eat);
	philo->time += philo->time_eat;
	ft_put_forks(philo);
	philo->now_count_eat++;
	return (1);
}

int	ft_sleap(t_philos *philo)
{
	if (philo->stop)
		return (0);
	pthread_mutex_lock(philo->entry_point);
	printf("%ld %d is sleeping\n", philo->time - philo->time_start, philo->name);
	pthread_mutex_unlock(philo->entry_point);
	castom_usleep(philo->time_sleap);
	philo->time += philo->time_sleap;
	return (1);
}