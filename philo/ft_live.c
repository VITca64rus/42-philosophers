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
	philo->last_eat = philo->time;
	pthread_mutex_lock(philo->entry_point);
	if (philo->stop)
	{
		pthread_mutex_unlock(philo->entry_point);
		return (0);
	}
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
	pthread_mutex_lock(philo->entry_point);
	if (philo->stop)
	{
		pthread_mutex_unlock(philo->entry_point);
		return (0);
	}
	printf("%ld %d is sleeping\n", philo->time - philo->time_start, philo->name);
	pthread_mutex_unlock(philo->entry_point);
	castom_usleep(philo->time_sleap);
	philo->time += philo->time_sleap;
	return (1);
}

int	ft_take_fork(t_philos *philo)
{
	struct timeval tv;
	int				res;

	res = 1;
	if (philo->right_fork > philo->left_fork)
	{
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		gettimeofday(&tv, NULL);
		philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		res = ft_custom_printf(philo, "has taken a fork");
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		gettimeofday(&tv, NULL);
		philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		res = ft_custom_printf(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		gettimeofday(&tv, NULL);
		philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		res = ft_custom_printf(philo, "has taken a fork");
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		gettimeofday(&tv, NULL);
		philo->time = tv.tv_sec * 1000 + tv.tv_usec/1000;
		res = ft_custom_printf(philo, "has taken a fork");
	}
	return (res);
}
