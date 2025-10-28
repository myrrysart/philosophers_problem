/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 06:56:12 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/28 12:39:33 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	mutex_error(t_philo_system *sim)
{
	sim->sim_state |= MUTEX_ERROR;
	return (1);
}

int	init_mutexes(t_philo_system *sim)
{
	int	i;

	i = 0;
	sim->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* (sim->nb_philos));
	if (!sim->forks)
		return (mutex_error(sim));
	while (i < sim->nb_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (mutex_error(sim));
		i++;
		sim->mutex_initiated++;
	}
	if (pthread_mutex_init(&sim->output_mutex, NULL) != 0)
		return (mutex_error(sim));
	sim->output_mutex_initiated++;
	return (0);
}
