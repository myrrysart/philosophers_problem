/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:15:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/24 12:30:27 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	sync_startup_wait(t_philosopher *philosopher, long long *start_ms)
{
	while (1)
	{
		pthread_mutex_lock(&philosopher->system->state_mutex);
		if (philosopher->system->sim_state & PHILO_ERROR)
		{
			pthread_mutex_unlock(&philosopher->system->state_mutex);
			return (1);
		}
		else if (philosopher->system->sim_state == RUNNING)
		{
			*start_ms = philosopher->system->start_time;
			pthread_mutex_unlock(&philosopher->system->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&philosopher->system->state_mutex);
		usleep(100);
	}
	return (0);
}

static int	sync_start(t_philosopher *philosopher)
{
	long long	start_ms;
	long long	now;

	if (sync_startup_wait(philosopher, &start_ms))
		return (1);
	now = get_time();
	if (now < start_ms)
		precise_sleep(start_ms - now);
	pthread_mutex_lock(&philosopher->lock);
	philosopher->last_meal_time = start_ms;
	philosopher->next_deadline_ms = start_ms + philosopher->system->time_to_die;
	pthread_mutex_unlock(&philosopher->lock);
	return (0);
}

static void	handle_single_philo(t_philosopher *philosopher)
{
	print_action(philosopher, "has taken a fork");
	precise_sleep(philosopher->system->time_to_die);
}

static void	run_cycle(t_philosopher *philosopher)
{
	philo_think(philosopher);
	philo_eat(philosopher);
	if (should_continue(philosopher))
		philo_sleep(philosopher);
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
	if (sim->nb_philos == 1)
	{
		handle_single_philo(philosopher);
		return (NULL);
	}
	while (should_continue(philosopher))
		run_cycle(philosopher);
	return (NULL);
}
