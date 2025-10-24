/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 06:56:12 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/24 16:30:27 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int clean_after_mutex_failure(t_philo_system *sim, int i, int flag)
{
	int	j;

	if (flag > 2)
		j = sim->nb_philos - 1;
	else
		j = i - 1;
	i =  i - 1;
	while (i > -1)
	{
		if (j > -1)
			pthread_mutex_destroy(&sim->forks[j]);
		if (flag > 2)
			pthread_mutex_destroy(&sim->philosophers[i].lock);
		j--;
		i--;
	}
	if (flag > 0)
		pthread_mutex_destroy(&sim->state_mutex);
	if (flag > 1)
		pthread_mutex_destroy(&sim->output_mutex);
	return (1);
}

int	init_mutexes(t_philo_system *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (clean_after_mutex_failure(sim, i, 0));
		i++;
	}
	if (pthread_mutex_init(&sim->output_mutex, NULL) != 0)
		return (clean_after_mutex_failure(sim, i, 1));
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (clean_after_mutex_failure(sim, i, 2));
	i = 0;
	while (i < sim->nb_philos)
	{
		if (pthread_mutex_init(&sim->philosophers[i].lock, NULL) != 0)
			return (clean_after_mutex_failure(sim, i, 3));
		i++;
	}
	return (0);
}
