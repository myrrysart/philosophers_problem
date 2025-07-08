/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:49 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/24 11:31:57 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philo);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_of_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (1);
	return (0);
}

int	init_data(t_data *data, char **argv, int argc)
{
	data->nb_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->nb_times_to_eat = ft_atoi(argv[5]);
	else
		data->nb_times_to_eat = -1;
	data->someone_dead = 0;
	data->meals_finished = 0;
	data->start_time = get_time();
	if (init_mutexes(data) != 0)
		return (1);
	return (0);
}

int	init_philos(t_philo **philos, t_data *data)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->nb_of_philo);
	if (!*philos)
		return (1);
	i = 0;
	while (i < data->nb_of_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].eating = 0;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].left_fork = i;
		(*philos)[i].right_fork = (i + 1) % data->nb_of_philo;
		(*philos)[i].last_meal = data->start_time;
		(*philos)[i].data = data;
		i++;
	}
	return (0);
}

void	cleanup(t_data *data, t_philo *philos)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->nb_of_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->write_lock);
	if (philos)
		free(philos);
}
