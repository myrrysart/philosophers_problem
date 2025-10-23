/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 06:56:12 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 06:56:18 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutexes(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		if (pthread_mutex_init(&philo->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&philo->output_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->state_mutex, NULL) != 0)
		return (1);
	i = 0;
	while (i < philo->nb_philos)
	{
		if (pthread_mutex_init(&philo->philosophers[i].lock, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
