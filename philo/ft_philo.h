/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazelda <sazelda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:22:23 by sazelda           #+#    #+#             */
/*   Updated: 2022/02/12 17:07:23 by sazelda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILO_H
# define FT_PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct s_philosof
{
	int				name;
	long			time_eat;
	long			time_sleap;
	long			time_start;
	long			time_death;
	long			last_eat;
	unsigned int	left_fork;
	unsigned int	right_fork;
	long			time;
	bool			stop;
	int				count_eat;
	int				now_count_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*entry_point;
}	t_philos;

typedef struct s_data
{
	t_philos		*philosophers;
	pthread_mutex_t	*forks;
	int				count;
}	t_data;

int				ft_atoi(const char *str);
pthread_mutex_t	*ft_create_forks(int count);
t_philos		*ft_create_philosophers(t_data *data, char **argv, \
										int argc, pthread_mutex_t *entry_point);
void			ft_put_forks(t_philos *philo);
int				ft_eat(t_philos *philo);
int				ft_sleap(t_philos *philo);
void			castom_usleep(long time);
int				ft_take_fork(t_philos *philo);
int				ft_custom_printf(t_philos *philo, char *s);
int				ft_check(int argc, char **argv);
void			*moni(void *args);

#endif