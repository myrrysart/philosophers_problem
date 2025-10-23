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

static bool	sim_running(t_philo_system *philo)
{
	bool	running;

	pthread_mutex_lock(&philo->state_mutex);
	running = (philo->sim_state == RUNNING);
	pthread_mutex_unlock(&philo->state_mutex);
	return (running);
}

static bool	lock_second_and_check(t_philo_system *philo, t_philosopher *p,
		int first, int second)
{
	(void)first;
	pthread_mutex_lock(&philo->forks[second]);
	if (!sim_running(philo))
	{
		pthread_mutex_unlock(&philo->forks[second]);
		pthread_mutex_unlock(&philo->forks[p->frst_fork_id]);
		return (false);
	}
	print_action(p, "has taken a fork");
	return (true);
}

bool	try_acquire_forks(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	int				first;
	int				second;

	philo = philosopher->system;
	if (!sim_running(philo))
		return (false);
	if (philo->nb_philos == 1)
	{
		pthread_mutex_lock(&philo->forks[0]);
		print_action(philosopher, "has taken a fork");
		return (false);
	}
	first = philosopher->frst_fork_id;
	second = philosopher->scnd_fork_id;
	pthread_mutex_lock(&philo->forks[first]);
	if (!sim_running(philo))
	{
		pthread_mutex_unlock(&philo->forks[first]);
		return (false);
	}
	print_action(philosopher, "has taken a fork");
	return (lock_second_and_check(philo, philosopher, first, second));
}

void	release_forks(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	int				first;
	int				second;

	philo = philosopher->system;
	if (philo->nb_philos == 1)
	{
		pthread_mutex_unlock(&philo->forks[0]);
		return ;
	}
	first = philosopher->frst_fork_id;
	second = philosopher->scnd_fork_id;
	pthread_mutex_unlock(&philo->forks[second]);
	pthread_mutex_unlock(&philo->forks[first]);
}
