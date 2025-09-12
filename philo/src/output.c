/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:25:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/12 19:25:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_action(t_philosopher *philosopher, char *action)
{
	t_philo_system	*philo;
	long long		timestamp;

	philo = philosopher->system;
	pthread_mutex_lock(&philo->output_mutex);
	pthread_mutex_lock(&philo->state_mutex);
	if (philo->sim_state == RUNNING)
	{
		timestamp = get_time() - philo->start_time;
		printf("%lld %d %s\n", timestamp, philosopher->id + 1, action);
	}
	pthread_mutex_unlock(&philo->state_mutex);
	pthread_mutex_unlock(&philo->output_mutex);
}

void	print_death(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	long long		timestamp;

	philo = philosopher->system;
	pthread_mutex_lock(&philo->output_mutex);
	timestamp = get_time() - philo->start_time;
	printf("%lld %d died\n", timestamp, philosopher->id + 1);
	pthread_mutex_unlock(&philo->output_mutex);
}
