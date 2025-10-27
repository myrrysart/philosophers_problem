/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:58 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 17:14:51 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_and_check_if_all_are_done(t_philo_system *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_philos)
	{
		if (pthread_create(&sim->philosophers[i].thread, NULL, philo_routine,
				&sim->philosophers[i]))
		{
			sim->sim_state |= PHILO_THREAD_ERROR | PHILO_ERROR;
			printf("Error creating philosopher thread %d\n", i);
			return (i);
		}
		i++;
	}
	return (0);
}

static int	create_threads(t_philo_system *sim)
{
	int	i;

	i = create_and_check_if_all_are_done(sim);
	if (i)
		return (i);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
	{
		sim->sim_state |= PHILO_MONITOR_ERROR | PHILO_ERROR;
		printf("Error creating monitor thread\n");
		return (1);
	}
	sim->start_time = get_time() + 400;
	i = 0;
	while (i < sim->nb_philos)
		sim->philosophers[i++].start_time = sim->start_time;
	sim->sim_state = RUNNING;
	return (0);
}

static void	join_threads(t_philo_system *sim, int j)
{
	int	i;

	i = 0;
	if (j == NORMAL_EXIT)
	{
		while (i < sim->nb_philos)
			pthread_join(sim->philosophers[i++].thread, NULL);
		pthread_join(sim->monitor_thread, NULL);
		return ;
	}
	if (sim->sim_state & PHILO_THREAD_ERROR)
	{
		while (i < j)
			pthread_join(sim->philosophers[i++].thread, NULL);
		return ;
	}
	if (!(sim->sim_state & PHILO_MONITOR_ERROR))
		pthread_join(sim->monitor_thread, NULL);
	while (i < sim->nb_philos)
	{
		pthread_join(sim->philosophers[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philo_system	philo_system;
	t_philo_system	*sim;
	int				i;

	philo_system = (t_philo_system){};
	sim = &philo_system;
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (init_system(sim, argv, argc) == 1)
		return (1);
	i = create_threads(sim);
	if ((sim->sim_state & (PHILO_THREAD_ERROR | PHILO_MONITOR_ERROR)))
	{
		join_threads(sim, i);
		cleanup_system(sim);
		return (1);
	}
	join_threads(sim, NORMAL_EXIT);
	cleanup_system(sim);
	return (0);
}
