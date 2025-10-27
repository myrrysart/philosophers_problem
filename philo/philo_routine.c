/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:15:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 15:12:15 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	sync_startup_wait(t_philosopher *philosopher)
{
	int	st;

	while (1)
	{
		st = philosopher->system->sim_state;
		if (st & PHILO_ERROR)
			return (1);
		else if (st == RUNNING)
		{
			philosopher->start_time = philosopher->system->start_time;
			break ;
		}
		usleep(100);
	}
	return (0);
}

static int	sync_start(t_philosopher *p)
{
	long long	now;

	if (sync_startup_wait(p))
		return (1);
	now = get_time();
	if (now < p->start_time)
		precise_sleep(p->system, p->start_time - now);
	p->last_meal_time = p->start_time;
	p->next_deadline_ms = p->start_time + p->time_to_die;
	return (0);
}

static void	handle_single_philo(t_philosopher *philosopher)
{
	print_action(philosopher, "is thinking");
	pthread_mutex_lock(philosopher->frst_fork);
	print_action(philosopher, "has taken a fork");
	precise_sleep(philosopher->system, philosopher->time_to_die);
	pthread_mutex_unlock(philosopher->frst_fork);
}

bool	should_continue(t_philosopher *p)
{
	bool	res;

	res = (p->system->sim_state == RUNNING) && !(p->state & SATISFIED)
		&& !(p->state & DEAD);
	return (res);
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_philo_system	*sim;

	philosopher = (t_philosopher *)arg;
	sim = philosopher->system;
	if (sync_start(philosopher))
		return (NULL);
	if (!should_continue(philosopher))
		return (NULL);
	if (philosopher->nb_philos == 1)
	{
		handle_single_philo(philosopher);
		return (NULL);
	}
	while (should_continue(philosopher))
	{
		philo_think(philosopher);
		philo_eat(philosopher);
		philo_sleep(philosopher);
	}
	return (NULL);
}
