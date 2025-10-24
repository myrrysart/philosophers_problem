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

static int clean_after_mutex_failure(t_philo_system *philo, int i, int flag)
{
	int	j;

	if (flag > 2)
		j = philo->nb_philos - 1;
	else
		j = i - 1;
	i =  i - 1;
	while (i > -1)
	{
		if (j > -1)
			pthread_mutex_destroy(&philo->forks[j]);
		if (flag > 2)
			pthread_mutex_destroy(&philo->philosophers[i].lock);
		j--;
		i--;
	}
	if (flag > 0)
		pthread_mutex_destroy(&philo->state_mutex);
	if (flag > 1)
		pthread_mutex_destroy(&philo->output_mutex);
	return (1);
}

int	init_mutexes(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		if (pthread_mutex_init(&philo->forks[i], NULL) != 0)
			return (clean_after_mutex_failure(philo, i, 0));
		i++;
	}
	if (pthread_mutex_init(&philo->output_mutex, NULL) != 0)
		return (clean_after_mutex_failure(philo, i, 1));
	if (pthread_mutex_init(&philo->state_mutex, NULL) != 0)
		return (clean_after_mutex_failure(philo, i, 2));
	i = 0;
	while (i < philo->nb_philos)
	{
		if (pthread_mutex_init(&philo->philosophers[i].lock, NULL) != 0)
			return (clean_after_mutex_failure(philo, i, 3));
		i++;
	}
	return (0);
}
