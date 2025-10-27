/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:36:45 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 12:15:48 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	lock_second_and_check(t_philo_system *sim, t_philosopher *p)
{
	pthread_mutex_lock(p->scnd_fork);
	if (!(sim->sim_state == RUNNING))
	{
		pthread_mutex_unlock(p->frst_fork);
		pthread_mutex_unlock(p->scnd_fork);
		return (false);
	}
	print_action(p, "has taken a fork");
	return (true);
}

bool	try_acquire_forks(t_philosopher *philosopher)
{
	t_philo_system	*sim;

	sim = philosopher->system;
	if (!(sim->sim_state == RUNNING))
		return (false);
	pthread_mutex_lock(philosopher->frst_fork);
	if (!(sim->sim_state == RUNNING))
	{
		pthread_mutex_unlock(philosopher->frst_fork);
		return (false);
	}
	print_action(philosopher, "has taken a fork");
	return (lock_second_and_check(sim, philosopher));
}

void	release_forks(t_philosopher *p)
{
	pthread_mutex_unlock(p->frst_fork);
	pthread_mutex_unlock(p->scnd_fork);
}
