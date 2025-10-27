/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:49 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 17:36:20 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_args_into(t_parsed_args *out, char **argv, int argc)
{
	out->nb_philos = safe_atoi(argv[1]);
	out->time_to_die = safe_atoi(argv[2]);
	out->time_to_eat = safe_atoi(argv[3]);
	out->time_to_sleep = safe_atoi(argv[4]);
	if (argc == 6)
		out->target_meal_count = safe_atoi(argv[5]);
	else
		out->target_meal_count = -1;
	if (out->nb_philos <= 0 || out->nb_philos > INT_MAX)
		return (1);
	if (out->time_to_die <= 0 || out->time_to_eat <= 0
		|| out->time_to_sleep <= 0)
		return (1);
	if (argc == 6 && out->target_meal_count <= 0)
		return (1);
	return (0);
}

static void	init_one_philo(t_philo_system *s, int i, t_parsed_args *a)
{
	int	left_fork;
	int	right_fork;

	
	s->philosophers[i].system = s;
	s->philosophers[i].id = i;
	s->philosophers[i].nb_philos = s->nb_philos;
	s->philosophers[i].time_to_die = a->time_to_die;
	s->philosophers[i].time_to_eat = a->time_to_eat;
	s->philosophers[i].time_to_sleep = a->time_to_sleep;
	s->philosophers[i].target_meal_count = a->target_meal_count;
	left_fork = i;
	right_fork = (i + 1) % s->philosophers[i].nb_philos;
	if (left_fork < right_fork)
	{
		s->philosophers[i].frst_fork = &s->forks[left_fork];
		s->philosophers[i].scnd_fork = &s->forks[right_fork];
	}
	else
	{
		s->philosophers[i].frst_fork = &s->forks[right_fork];
		s->philosophers[i].scnd_fork = &s->forks[left_fork];
	}
	if (i % 2)
		s->philosophers[i].stagger_ms = s->philosophers[i].time_to_eat / 2;
}

static void	init_philosophers(t_philo_system *sim, t_parsed_args *a)
{
	int	i;

	i = 0;
	sim->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher) * sim->nb_philos);
	if (!sim->philosophers)
	{
		sim->sim_state |= MALLOC_ERROR;
		return ;
	}
	memset(sim->philosophers, 0, sizeof(t_philosopher) * sim->nb_philos);
	while (i < sim->nb_philos)
	{
		init_one_philo(sim, i, a);
		i++;
	}
}

int	init_system(t_philo_system *sim, char **argv, int argc)
{
	t_parsed_args	a;

	if (parse_args_into(&a, argv, argc) != 0)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	sim->nb_philos = a.nb_philos;
	sim->start_time = 0;
	sim->sim_state = WAITING;
	sim->satisfied_count = 0;
	if (init_mutexes(sim) != 0)
	{
		printf("Error: Failed to initialize mutexes\n");
		return (1);
	}
	init_philosophers(sim, &a);
	if (sim->sim_state & MALLOC_ERROR)
	{
		printf("Error: Failed to allocate philosophers\n");
		cleanup_system(sim);
		return (1);
	}
	return (0);
}

void	cleanup_system(t_philo_system *sim)
{
	int	i;

	if (sim->forks) {
		i = 0;
		while (i < sim->nb_philos) {
			pthread_mutex_destroy(&sim->forks[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&sim->output_mutex);
	free(sim->forks);
	sim->forks = NULL;
	free(sim->philosophers);
	sim->philosophers = NULL;
}
