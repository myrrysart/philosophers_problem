/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:20:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 14:29:54 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	monitor_startup_wait(t_philo_system *sim)
{
	long long	start_time;
	long long	now;
	int			st;

	start_time = sim->start_time;
	while (1)
	{
		st = sim->sim_state;
		if (st & PHILO_ERROR)
			return (1);
		else if (st == RUNNING)
		{
			now = get_time();
			if (now < start_time)
				precise_sleep(sim, start_time - now);
			break ;
		}
		usleep(100);
	}
	return (0);
}

static bool	check_completion(t_philo_system *sim)
{
	if (sim->philosophers[0].target_meal_count == -1)
		return (false);
	if (sim->satisfied_count == sim->nb_philos)
	{
		sim->sim_state = ALL_SATISFIED;
		return (true);
	}
	return (false);
}

static bool	philo_expired(t_philosopher *p, long long now)
{
	long long	deadline;

	deadline = p->next_deadline_ms;
	if (deadline == 0)
		return (false);
	return (now > deadline);
}

static bool	check_deaths(t_philo_system *sim)
{
	int			i;
	long long	current_time;

	current_time = get_time();
	i = 0;
	while (i < sim->nb_philos)
	{
		if (philo_expired(&sim->philosophers[i], current_time))
		{
			sim->sim_state = SOMEONE_DIED;
			sim->philosophers[i].state |= DEAD;
			print_death(&sim->philosophers[i]);
			return (true);
		}
		i++;
	}
	return (false);
}

void	*monitor_routine(void *arg)
{
	t_philo_system	*sim;

	sim = (t_philo_system *)arg;
	if (monitor_startup_wait(sim))
		return (NULL);
	while (1)
	{
		if (check_deaths(sim))
			break ;
		if (check_completion(sim))
			break ;
		usleep(1000);
	}
	return (NULL);
}
