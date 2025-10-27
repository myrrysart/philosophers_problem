/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 06:56:12 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 10:53:44 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	clean_after_mutex_failure(t_philo_system *sim, int i, int flag)
{
	int	j;

	j = i - 1;
	while (j > -1)
	{
		pthread_mutex_destroy(&sim->forks[j]);
		j--;
	}
	if (flag > 0)
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
	return (0);
}
