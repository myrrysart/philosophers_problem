/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:20:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 06:47:51 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_philo_system	*philo;
		long long	start_time;
		long long	now;

	philo = (t_philo_system *)arg;
	while(1)
	{
		pthread_mutex_lock(&philo->state_mutex);
		if (philo->sim_state & PHILO_ERROR)
		{
			pthread_mutex_unlock(&philo->state_mutex);
			return (NULL);
		}
		else if (philo->sim_state == RUNNING)
		{
			start_time = philo->start_time;
			pthread_mutex_unlock(&philo->state_mutex);
			now = get_time();
			if (now < start_time)
				precise_sleep(start_time - now);
			break ;
		}
		pthread_mutex_unlock(&philo->state_mutex);
		usleep(100);
	}
	while (1)
	{
		if (check_deaths(philo))
			break ;
		if (check_completion(philo))
			break ;
		usleep(1000);
	}
	return (NULL);
}

static bool	philo_expired(t_philosopher *p, long long now)
{
	long long	deadline;

	pthread_mutex_lock(&p->lock);
	deadline = p->next_deadline_ms;
	pthread_mutex_unlock(&p->lock);
	if (deadline == 0)
		return (false);
	return (now > deadline);
}

static void	signal_death(t_philo_system *philo, t_philosopher *p)
{
	pthread_mutex_lock(&philo->state_mutex);
	philo->sim_state = SOMEONE_DIED;
	pthread_mutex_unlock(&philo->state_mutex);
	pthread_mutex_lock(&p->lock);
	p->state |= DEAD;
	pthread_mutex_unlock(&p->lock);
	pthread_mutex_lock(&philo->output_mutex);
	print_death(p);
	pthread_mutex_unlock(&philo->output_mutex);
}

bool	check_deaths(t_philo_system *philo)
{
	int			i;
	long long	current_time;

	current_time = get_time();
	i = 0;
	while (i < philo->nb_philos)
	{
		if (philo_expired(&philo->philosophers[i], current_time))
		{
			signal_death(philo, &philo->philosophers[i]);
			return (true);
		}
		i++;
	}
	return (false);
}
