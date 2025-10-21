/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:36:45 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/21 18:36:45 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	fork_order(const t_philosopher *p, int *first, int *second)
{
	if ((p->id % 2) == 0)
	{
		*first = p->left_fork_id;
		*second = p->right_fork_id;
	}
	else
	{
		*first = p->right_fork_id;
		*second = p->left_fork_id;
	}
}

static bool	sim_running(t_philo_system *philo)
{
	bool	running;

	pthread_mutex_lock(&philo->state_mutex);
	running = (philo->sim_state == RUNNING);
	pthread_mutex_unlock(&philo->state_mutex);
	return (running);
}

bool	try_acquire_forks(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	int				first_fork;
	int				second_fork;

	philo = philosopher->system;
	if (!sim_running(philo))
		return (false);
	if (philo->nb_philos == 1)
	{
		pthread_mutex_lock(&philo->forks[0]);
		print_action(philosopher, "has taken a fork");
		return (false);
	}
	fork_order(philosopher, &first_fork, &second_fork);
	pthread_mutex_lock(&philo->forks[first_fork]);
	if (!sim_running(philo))
	{
		pthread_mutex_unlock(&philo->forks[first_fork]);
		return (false);
	}
	print_action(philosopher, "has taken a fork");
	pthread_mutex_lock(&philo->forks[second_fork]);
	print_action(philosopher, "has taken a fork");
	return (true);
}

void	release_forks(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	int				first_fork;
	int				second_fork;

	philo = philosopher->system;
	if (philo->nb_philos == 1)
	{
		pthread_mutex_unlock(&philo->forks[0]);
		return ;
	}
	fork_order(philosopher, &first_fork, &second_fork);
	pthread_mutex_unlock(&philo->forks[first_fork]);
	pthread_mutex_unlock(&philo->forks[second_fork]);
}
