/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:15:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 05:00:06 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	sync_start(t_philosopher *philosopher)
{
	long long	delay;

	delay = philosopher->system->start_time - get_time();
	if (delay > 0)
		precise_sleep(delay);
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
	t_philo_system	*philo;

	philosopher = (t_philosopher *)arg;
	philo = philosopher->system;
	sync_start(philosopher);
	if (philo->nb_philos == 1)
	{
		handle_single_philo(philosopher);
		return (NULL);
	}
	while (should_continue(philosopher))
		run_cycle(philosopher);
	return (NULL);
}
