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
	struct s_table	*table;
	int			name;
	long long	time_eat;
	long long	time_sleap;
	long long	time_death;
	long long	last_eat;
	unsigned	left_fork;
	unsigned	right_fork;
}	t_philos;

typedef struct s_table
{
	t_philos		philosopher;
	pthread_mutex_t	*forks;
	bool			stop;
}	t_table;

pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;

int	ft_atoi(const char *str);

#endif