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

static int	parse_args(t_philo_system *sim, char **argv, int argc)
{
	sim->nb_philos = safe_atoi(argv[1]);
	sim->time_to_die = safe_atoi(argv[2]);
	sim->time_to_eat = safe_atoi(argv[3]);
	sim->time_to_sleep = safe_atoi(argv[4]);
	if (argc == 6)
		sim->target_meal_count = safe_atoi(argv[5]);
	else
		sim->target_meal_count = -1;
	if (sim->nb_philos <= 0 || sim->nb_philos > MAX_PHILOS)
		return (1);
	if (sim->time_to_die <= 0 || sim->time_to_eat <= 0
		|| sim->time_to_sleep <= 0)
		return (1);
	if (argc == 6 && sim->target_meal_count <= 0)
		return (1);
	return (0);
}

static void	init_one_philo(t_philo_system *s, int i)
{
	s->philosophers[i].system = s;
	s->philosophers[i].id = i;
	s->philosophers[i].last_meal_time = 0;
	s->philosophers[i].next_deadline_ms = 0;
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

static void	init_philosophers(t_philo_system *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_philos)
	{
		init_one_philo(sim, i);
		i++;
	}
}

int	init_system(t_philo_system *sim, char **argv, int argc)
{
	if (parse_args(sim, argv, argc) != 0)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	sim->eat_half = sim->time_to_eat / 2;
	sim->die_minus_eat = sim->time_to_die - sim->time_to_eat;
	sim->start_time = 0;
	sim->sim_state = WAITING;
	sim->satisfied_count = 0;
	init_philosophers(sim);
	if (init_mutexes(sim) != 0)
	{
		printf("Error: Failed to initialize mutexes\n");
		return (1);
	}
	return (0);
}

void	cleanup_system(t_philo_system *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_philos)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		pthread_mutex_destroy(&sim->philosophers[i].lock);
		i++;
	}
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_mutex_destroy(&sim->output_mutex);
}
