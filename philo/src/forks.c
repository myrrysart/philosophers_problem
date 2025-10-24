/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:36:45 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 07:19:34 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	sim_running(t_philo_system *sim)
{
	bool	running;

	pthread_mutex_lock(&sim->state_mutex);
	running = (sim->sim_state == RUNNING);
	pthread_mutex_unlock(&sim->state_mutex);
	return (running);
}

static bool	lock_second_and_check(t_philo_system *sim, t_philosopher *p,
		int first, int second)
{
	(void)first;
	pthread_mutex_lock(&sim->forks[second]);
	if (!sim_running(sim))
	{
		pthread_mutex_unlock(&sim->forks[second]);
		pthread_mutex_unlock(&sim->forks[p->frst_fork_id]);
		return (false);
	}
	print_action(p, "has taken a fork");
	return (true);
}

bool	try_acquire_forks(t_philosopher *philosopher)
{
	t_philo_system	*sim;
	int				first;
	int				second;

	sim = philosopher->system;
	if (!sim_running(sim))
		return (false);
	if (sim->nb_philos == 1)
	{
		pthread_mutex_lock(&sim->forks[0]);
		print_action(philosopher, "has taken a fork");
		return (false);
	}
	first = philosopher->frst_fork_id;
	second = philosopher->scnd_fork_id;
	pthread_mutex_lock(&sim->forks[first]);
	if (!sim_running(sim))
	{
		pthread_mutex_unlock(&sim->forks[first]);
		return (false);
	}
	print_action(philosopher, "has taken a fork");
	return (lock_second_and_check(sim, philosopher, first, second));
}

void	release_forks(t_philosopher *philosopher)
{
	t_philo_system	*sim;
	int				first;
	int				second;

	sim = philosopher->system;
	if (sim->nb_philos == 1)
	{
		pthread_mutex_unlock(&sim->forks[0]);
		return ;
	}
	first = philosopher->frst_fork_id;
	second = philosopher->scnd_fork_id;
	pthread_mutex_unlock(&sim->forks[second]);
	pthread_mutex_unlock(&sim->forks[first]);
}
