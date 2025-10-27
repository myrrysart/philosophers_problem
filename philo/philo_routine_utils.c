/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 05:00:15 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 22:44:13 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	update_meal_data(t_philosopher *philosopher)
{
	long long	now;

	now = get_time();
	philosopher->last_meal_time = now;
	philosopher->next_deadline_ms = now + philosopher->time_to_die;
	philosopher->meal_count++;
	if (philosopher->meal_count == 1)
		philosopher->stagger_ms = 0;
	if (philosopher->target_meal_count != -1
		&& philosopher->meal_count >= philosopher->target_meal_count)
	{
		philosopher->state |= SATISFIED;
		philosopher->system->satisfied_count++;
	}
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
		precise_sleep(philosopher->system, philosopher->time_to_eat);
		release_forks(philosopher);
	}
}

void	philo_sleep(t_philosopher *philosopher)
{
	print_action(philosopher, "is sleeping");
	precise_sleep(philosopher->system, philosopher->time_to_sleep);
}

static void	apply_jitter(t_philosopher *p)
{
	long long	now;
	long long	time_until_death;

	now = get_time();
	time_until_death = p->next_deadline_ms - now;
	if (time_until_death > 20)
	{
		usleep(12000 + (p->id % 4) * 1000);
	}
}

void	philo_think(t_philosopher *philosopher)
{
	print_action(philosopher, "is thinking");
	if (philosopher->stagger_ms > 0)
		precise_sleep(philosopher->system, philosopher->stagger_ms);
	if (!(philosopher->meal_count == 0))
		apply_jitter(philosopher);
}
