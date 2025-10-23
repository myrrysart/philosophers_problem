/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 05:00:15 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 12:06:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	should_continue(t_philosopher *p)
{
	unsigned int	sim;
	unsigned int	flags;

	pthread_mutex_lock(&p->system->state_mutex);
	sim = p->system->sim_state;
	pthread_mutex_unlock(&p->system->state_mutex);
	pthread_mutex_lock(&p->lock);
	flags = p->state;
	pthread_mutex_unlock(&p->lock);
	return ((sim == RUNNING) && ((flags & SATISFIED) == 0));
}

void	philo_eat(t_philosopher *philosopher)
{
	if (try_acquire_forks(philosopher))
	{
		if (!should_continue(philosopher))
		{
			release_forks(philosopher);
			return ;
		}
		update_meal_data(philosopher);
		print_action(philosopher, "is eating");
		precise_sleep(philosopher->system->time_to_eat);
		release_forks(philosopher);
	}
}

void	philo_sleep(t_philosopher *philosopher)
{
	print_action(philosopher, "is sleeping");
	precise_sleep(philosopher->system->time_to_sleep);
}

static void	apply_jitter(t_philosopher *p)
{
	long long	now;
	long long	deadline;
	long long	remaining;
	long long	js;

	if (p->jitter_us <= 0)
		return ;
	now = get_time();
	pthread_mutex_lock(&p->lock);
	deadline = p->next_deadline_ms;
	pthread_mutex_unlock(&p->lock);
	remaining = deadline - now;
	if (remaining > (p->system->time_to_eat + 2))
	{
		js = p->jitter_us;
		if (js > (remaining - (p->system->time_to_eat + 2)) * 1000)
			js = (remaining - (p->system->time_to_eat + 2)) * 1000;
		if (js > 0)
			usleep((useconds_t)js);
	}
}

void	philo_think(t_philosopher *philosopher)
{
	print_action(philosopher, "is thinking");
	if (philosopher->stagger_ms > 0)
		precise_sleep(philosopher->stagger_ms);
	apply_jitter(philosopher);
}
