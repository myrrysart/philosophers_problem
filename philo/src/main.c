/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:58 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 18:46:53 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_threads(t_philo_system *philo)
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
	if (pthread_create(&philo->monitor_thread, NULL, monitor_routine,
			philo) != 0)
	{
		printf("Error creating monitor thread\n");
		return (1);
	}
	pthread_mutex_lock(&philo->state_mutex);
	philo->start_time = get_time() + 200;
	philo->sim_state = RUNNING;
	pthread_mutex_unlock(&philo->state_mutex);
	i = 0;
	while (i < philo->nb_philos;)
	{
		pthread_mutex_lock(&philo->philosophers[i].lock);
		philo->philosophers[i].last_meal_time = philo->start_time;
		philo->philosophers[i].next_deadline_ms = philo->start_time + philo->time_to_die;
		pthread_mutex_unlock(&philo->philosophers[i].lock);
		i++;
	}
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
