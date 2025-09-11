/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:58 by jyniemit          #+#    #+#             */
/*   Updated: 2025/07/09 13:03:36 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	create_threads(t_philo *philos, pthread_t *monitor_thread)
{
	int	i;

	i = -1;
	while (++i < philos[0].data->nb_of_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_routine,
				&philos[i]) != 0)
		{
			pthread_mutex_lock(&philos[0].data->death_lock);
			philos[0].data->someone_dead = 1;
			pthread_mutex_unlock(&philos[0].data->death_lock);
			while(--i >= 0)
				pthread_join(philos[i].thread, NULL);
			return (1);
		}
	}
	if (pthread_create(monitor_thread, NULL, &monitor, philos) != 0)
		{
			pthread_mutex_lock(&philos[0].data->death_lock);
			philos[0].data->someone_dead = 1;
			pthread_mutex_unlock(&philos[0].data->death_lock);
			i = -1;
			while(++i < philos[0].data->nb_of_philo)
				pthread_join(philos[i].thread, NULL);
			return (1);
		}
	return (0);
}

static void	join_threads(t_philo *philos, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < philos[0].data->nb_of_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	monitor_thread;

	if (check_args(argc, argv) == 1)
		return (1);
	if (init_data(&data, argv, argc) == 1)
		return (1);
	if (init_philos(&philos, &data) == 1)
		return (1);
	if (create_threads(philos, &monitor_thread) == 1)
		return (1);
	join_threads(philos, monitor_thread);
	cleanup(&data, philos);
	return (0);
}
