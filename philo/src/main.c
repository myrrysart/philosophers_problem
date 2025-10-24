/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:58 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/24 15:01:13 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_and_check_if_all_are_done(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		if (pthread_create(&philo->philosophers[i].thread, NULL, philo_routine,
				&philo->philosophers[i]))
		{
			pthread_mutex_lock(&philo->state_mutex);
			philo->sim_state = PHILO_ERROR;
			pthread_mutex_unlock(&philo->state_mutex);
			printf("Error creating philosopher thread %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	create_threads(t_philo_system *philo)
{
	if (create_and_check_if_all_are_done(philo))
		return (1);
	if (pthread_create(&philo->monitor_thread, NULL, monitor_routine,
			philo) != 0)
	{
		pthread_mutex_lock(&philo->state_mutex);
		philo->sim_state = PHILO_ERROR;
		pthread_mutex_unlock(&philo->state_mutex);
		printf("Error creating monitor thread\n");
		return (1);
	}
	pthread_mutex_lock(&philo->state_mutex);
	philo->start_time = get_time() + 200;
	philo->sim_state = RUNNING;
	pthread_mutex_unlock(&philo->state_mutex);
	return (0);
}

static void	join_threads(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		pthread_join(philo->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(philo->monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_philo_system	philo_system;
	t_philo_system	*philo;

	philo_system = (t_philo_system){};
	philo = &philo_system;
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid arguments\\n");
		return (1);
	}
	if (init_system(philo, argv, argc) == 1)
		return (1);
	if (create_threads(philo) == 1)
	{
		join_threads(philo);
		cleanup_system(philo);
		return (1);
	}
	join_threads(philo);
	cleanup_system(philo);
	return (0);
}
