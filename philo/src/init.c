/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:49 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 07:17:09 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void	init_one_philo(t_philo_system *s, int i)
{
	s->philosophers[i].system = s;
	s->philosophers[i].id = i;
	s->philosophers[i].last_meal_time = s->start_time;
	s->philosophers[i].next_deadline_ms = s->start_time + s->time_to_die;
	s->philosophers[i].left_fork_id = i;
	s->philosophers[i].right_fork_id = (i + 1) % s->nb_philos;
	s->philosophers[i].is_odd = (i % 2);
	if (s->philosophers[i].left_fork_id < s->philosophers[i].right_fork_id)
	{
		s->philosophers[i].frst_fork_id = s->philosophers[i].left_fork_id;
		s->philosophers[i].scnd_fork_id = s->philosophers[i].right_fork_id;
	}
	else
	{
		s->philosophers[i].frst_fork_id = s->philosophers[i].right_fork_id;
		s->philosophers[i].scnd_fork_id = s->philosophers[i].left_fork_id;
	}
	s->philosophers[i].frst_fork = &s->forks[s->philosophers[i].frst_fork_id];
	s->philosophers[i].scnd_fork = &s->forks[s->philosophers[i].scnd_fork_id];
	if (s->philosophers[i].is_odd)
		s->philosophers[i].stagger_ms = s->time_to_eat / 2;
	s->philosophers[i].jitter_us = 200 * (1 + (i % 4));
}

static void	init_philosophers(t_philo_system *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		init_one_philo(philo, i);
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
	philo->eat_half = philo->time_to_eat / 2;
	philo->die_minus_eat = philo->time_to_die - philo->time_to_eat;
	philo->start_time = get_time() + 50;
	philo->sim_state = RUNNING;
	philo->satisfied_count = 0;
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
		pthread_mutex_destroy(&philo->philosophers[i].lock);
		i++;
	}
	pthread_mutex_destroy(&philo->state_mutex);
	pthread_mutex_destroy(&philo->output_mutex);
}
