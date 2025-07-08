/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:05:36 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/23 14:28:40 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

typedef struct s_data
{
	int				nb_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	int				someone_dead;
	int				meals_finished;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	write_lock;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	int				left_fork;
	int				right_fork;
	long long		last_meal;
	t_data			*data;
}	t_philo;

int			main(int argc, char **argv);
int			check_args(int argc, char **argv);
int			ft_atoi(const char *str);
int			init_data(t_data *data, char **argv, int argc);
int			init_philos(t_philo **philos, t_data *data);
int			init_forks(t_data *data);
void		*philo_routine(void *philosopher);
void		*monitor(void *arg);
long long	get_time(void);
void		ft_usleep(long long milliseconds);
void		ft_usleep_check(long long milliseconds, t_data *data);
void		print_message(char *str, t_philo *philo);
void		cleanup(t_data *data, t_philo *philos);
int			should_continue(t_data *data);

#endif
