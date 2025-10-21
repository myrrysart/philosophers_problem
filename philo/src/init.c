/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:49 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/21 17:03:39 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	init_mutexes(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		if (pthread_mutex_init(&philo->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&philo->state_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->output_mutex, NULL) != 0)
		return (1);
	return (0);
}

static int	parse_args(t_philo_system *philo, char **argv, int argc)
{
	philo->nb_philos = safe_atoi(argv[1]);
	philo->time_to_die = safe_atoi(argv[2]);
	philo->time_to_eat = safe_atoi(argv[3]);
	philo->time_to_sleep = safe_atoi(argv[4]);
	if (argc == 6)
		philo->target_meal_count = safe_atoi(argv[5]);
	else
		philo->target_meal_count = -1;
	if (philo->nb_philos <= 0 || philo->nb_philos > MAX_PHILOS)
		return (1);
	if (philo->time_to_die <= 0 || philo->time_to_eat <= 0
		|| philo->time_to_sleep <= 0)
		return (1);
	if (argc == 6 && philo->target_meal_count <= 0)
		return (1);
	return (0);
}

static void	init_philosophers(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		philo->philosophers[i].system = philo;
		philo->philosophers[i].id = i;
		philo->philosophers[i].state = 0;
		philo->philosophers[i].last_meal_time = philo->start_time;
		philo->philosophers[i].meal_count = 0;
		philo->philosophers[i].left_fork_id = i;
		philo->philosophers[i].right_fork_id = (i + 1) % philo->nb_philos;
		i++;
	}
}

int	init_system(t_philo_system *philo, char **argv, int argc)
{
	if (parse_args(philo, argv, argc) != 0)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	philo->start_time = get_time() + 50;
	philo->sim_state = RUNNING;
	init_philosophers(philo);
	if (init_mutexes(philo) != 0)
	{
		printf("Error: Failed to initialize mutexes\n");
		return (1);
	}
	return (0);
}

void	cleanup_system(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->state_mutex);
	pthread_mutex_destroy(&philo->output_mutex);
}
