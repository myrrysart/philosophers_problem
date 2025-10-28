/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:25:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/27 13:44:03 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philosopher *philosopher, char *action)
{
	t_philo_system	*sim;
	long long		timestamp;

	sim = philosopher->system;
	if (sim->sim_state == RUNNING)
	{
		pthread_mutex_lock(&sim->output_mutex);
		timestamp = get_time() - philosopher->start_time;
		if (sim->sim_state == RUNNING)
			printf("%lld %d %s\n", timestamp, philosopher->id + 1, action);
		pthread_mutex_unlock(&sim->output_mutex);
	}
}

void	print_death(t_philosopher *philosopher)
{
	t_philo_system	*sim;
	long long		timestamp;

	sim = philosopher->system;
	pthread_mutex_lock(&sim->output_mutex);
	timestamp = get_time() - philosopher->start_time;
	printf("%lld %d died\n", timestamp, philosopher->id + 1);
	pthread_mutex_unlock(&sim->output_mutex);
}
