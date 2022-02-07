#ifndef FT_PHILO_H
# define FT_PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
#include <unistd.h>
# include <stdbool.h>
# include <stdio.h> //FIX_ME

typedef struct s_philosof
{
	int			name;
	long		time_eat;
	long		time_sleap;
	long		time_start;
	long		time_death;
	long		last_eat;
	unsigned	left_fork;
	unsigned	right_fork;
	long		time;
	bool		stop;
	int			count_eat;
	int			now_count_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t *entry_point;
}	t_philos;


typedef struct s_data
{
	t_philos		*philosophers;
	pthread_mutex_t	*forks;
	int				count;
}	t_data;

int	ft_atoi(const char *str);
pthread_mutex_t *ft_create_forks(int count);
t_philos	*ft_create_philosophers(int count, pthread_mutex_t *forks, char **argv, int argc, pthread_mutex_t *entry_point);
void	ft_put_forks(t_philos *philo);
int		ft_eat(t_philos *philo);
int		ft_sleap(t_philos *philo);
void castom_usleep(long time);

#endif