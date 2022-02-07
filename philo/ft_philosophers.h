#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

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
	long		time_death;
	long		last_eat;
	unsigned	left_fork;
	unsigned	right_fork;
	long		time;
	bool			stop;
	int			count_eat;
	int			now_count_eat;
	pthread_mutex_t	*forks;
}	t_philos;


typedef struct s_data
{
	t_philos		*philosophers;
	pthread_mutex_t	*forks;
	int				count;
}	t_data;


pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;

int	ft_atoi(const char *str);

#endif