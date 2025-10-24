/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:25:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/22 19:48:17 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philosopher *philosopher, char *action)
{
	t_philo_system *sim;
	long long		timestamp;
	bool			ok;

	sim = philosopher->system;
	pthread_mutex_lock(&sim->state_mutex);
	pthread_mutex_lock(&sim->output_mutex);
	ok = (sim->sim_state == RUNNING);
	if (ok)
	{
		timestamp = get_time() - sim->start_time;
		printf("%lld %d %s\n", timestamp, philosopher->id + 1, action);
	}
	pthread_mutex_unlock(&sim->output_mutex);
	pthread_mutex_unlock(&sim->state_mutex);
}

void	print_death(t_philosopher *philosopher)
{
	t_philo_system *sim;
	long long		timestamp;

	sim = philosopher->system;
	timestamp = get_time() - sim->start_time;
	printf("%lld %d died\n", timestamp, philosopher->id + 1);
}
