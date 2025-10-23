/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:10:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 03:44:47 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	mark_satisfied_if_needed(t_philo_system *philo, t_philosopher *p)
{
	int	newly;

	newly = 0;
	pthread_mutex_lock(&p->lock);
	if (philo->target_meal_count != -1
		&& p->meal_count >= philo->target_meal_count
		&& p->satisfied_marked == 0)
	{
		p->satisfied_marked = 1;
		p->state |= SATISFIED;
		newly = 1;
	}
	pthread_mutex_unlock(&p->lock);
	if (newly)
	{
		pthread_mutex_lock(&philo->state_mutex);
		philo->satisfied_count += 1;
		pthread_mutex_unlock(&philo->state_mutex);
	}
	return (newly);
}

void	update_meal_data(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	long long		now;

	philo = philosopher->system;
	now = get_time();
	pthread_mutex_lock(&philosopher->lock);
	philosopher->last_meal_time = now;
	philosopher->next_deadline_ms = now + philo->time_to_die;
	philosopher->meal_count += 1;
	if (philosopher->meal_count == 1)
		philosopher->stagger_ms = 0;
	pthread_mutex_unlock(&philosopher->lock);
	(void)mark_satisfied_if_needed(philo, philosopher);
}

bool	check_completion(t_philo_system *philo)
{
	if (philo->target_meal_count == -1)
		return (false);
	pthread_mutex_lock(&philo->state_mutex);
	if (philo->satisfied_count == philo->nb_philos)
	{
		philo->sim_state = ALL_SATISFIED;
		pthread_mutex_unlock(&philo->state_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->state_mutex);
	return (false);
}
