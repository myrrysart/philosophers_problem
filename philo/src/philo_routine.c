/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:15:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/21 18:13:51 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	should_continue(t_philo_system *philo)
{
	bool	result;

	pthread_mutex_lock(&philo->state_mutex);
	result = (philo->sim_state == RUNNING);
	pthread_mutex_unlock(&philo->state_mutex);
	return (result);
}

static void	philo_eat(t_philosopher *philosopher)
{
	bool	running;

	if (try_acquire_forks(philosopher))
	{
		pthread_mutex_lock(&philosopher->system->state_mutex);
		running = (philosopher->system->sim_state == RUNNING);
		pthread_mutex_unlock(&philosopher->system->state_mutex);
		if (!running)
		{
			release_forks(philosopher);
			return ;
		}
		update_meal_data(philosopher);
		print_action(philosopher, "is eating");
		precise_sleep(philosopher->system->time_to_eat);
		release_forks(philosopher);
	}
}

static void	philo_sleep(t_philosopher *philosopher)
{
	print_action(philosopher, "is sleeping");
	precise_sleep(philosopher->system->time_to_sleep);
}

static void	philo_think(t_philosopher *philosopher)
{
	print_action(philosopher, "is thinking");
	if ((philosopher->system->nb_philos % 2) == 1)
		precise_sleep(philosopher->system->time_to_eat / 10);
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_philo_system	*philo;
	bool			first;

	philosopher = (t_philosopher *)arg;
	philo = philosopher->system;
	while (get_time() < philo->start_time)
		usleep(200);
	if (philo->nb_philos == 1)
	{
		print_action(philosopher, "has taken a fork");
		precise_sleep(philo->time_to_die);
		return (NULL);
	}
	first = true;
	while (should_continue(philo))
	{
		philo_think(philosopher);
		first = false;
		philo_eat(philosopher);
		if (!should_continue(philo))
			break ;
		philo_sleep(philosopher);
	}
	return (NULL);
}
