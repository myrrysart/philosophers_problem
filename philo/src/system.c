/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:10:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 08:20:11 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_meal_data(t_philosopher *philosopher)
{
	t_philo_system	*sim;
	long long		now;

	sim = philosopher->system;
	now = get_time();
	pthread_mutex_lock(&philosopher->lock);
	philosopher->last_meal_time = now;
	philosopher->next_deadline_ms = now + sim->time_to_die;
	philosopher->meal_count += 1;
	if (philosopher->meal_count == 1)
		philosopher->stagger_ms = 0;
	if (sim->target_meal_count != -1
		&& philosopher->meal_count >= sim->target_meal_count
		&& philosopher->satisfied_marked == 0)
	{
		philosopher->satisfied_marked = 1;
		philosopher->state |= SATISFIED;
		pthread_mutex_unlock(&philosopher->lock);
		pthread_mutex_lock(&sim->state_mutex);
		sim->satisfied_count++;
		pthread_mutex_unlock(&sim->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&philosopher->lock);
}

bool	check_completion(t_philo_system *sim)
{
	if (sim->target_meal_count == -1)
		return (false);
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->satisfied_count == sim->nb_philos)
	{
		sim->sim_state = ALL_SATISFIED;
		pthread_mutex_unlock(&sim->state_mutex);
		return (true);
	}
	pthread_mutex_unlock(&sim->state_mutex);
	return (false);
}
