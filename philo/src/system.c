/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:10:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/21 17:59:55 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	try_acquire_forks(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	int				first_fork;
	int				second_fork;

	philo = philosopher->system;
	if (philo->nb_philos == 1)
	{
		pthread_mutex_lock(&philo->forks[0]);
		print_action(philosopher, "has taken a fork");
		return (false);
	}
	first_fork = philosopher->left_fork_id;
	second_fork = philosopher->right_fork_id;
	if (first_fork > second_fork)
	{
		first_fork = second_fork;
		second_fork = philosopher->left_fork_id;
	}
	pthread_mutex_lock(&philo->forks[first_fork]);
	print_action(philosopher, "has taken a fork");
	pthread_mutex_lock(&philo->forks[second_fork]);
	print_action(philosopher, "has taken a fork");
	return (true);
}

void	release_forks(t_philosopher *philosopher)
{
	t_philo_system	*philo;
	int				first_fork;
	int				second_fork;

	philo = philosopher->system;
	if (philo->nb_philos == 1)
	{
		pthread_mutex_unlock(&philo->forks[0]);
		return ;
	}
	first_fork = philosopher->left_fork_id;
	second_fork = philosopher->right_fork_id;
	if (first_fork > second_fork)
	{
		first_fork = second_fork;
		second_fork = philosopher->left_fork_id;
	}
	pthread_mutex_unlock(&philo->forks[first_fork]);
	pthread_mutex_unlock(&philo->forks[second_fork]);
}

void	update_meal_data(t_philosopher *philosopher)
{
	t_philo_system	*philo;

	philo = philosopher->system;
	pthread_mutex_lock(&philo->state_mutex);
	philosopher->last_meal_time = get_time();
	philosopher->meal_count++;
	pthread_mutex_unlock(&philo->state_mutex);
}

bool	check_deaths(t_philo_system *philo)
{
	int			i;
	long long	current_time;
	long long	time_since_meal;

	current_time = get_time();
	i = 0;
	while (i < philo->nb_philos)
	{
		pthread_mutex_lock(&philo->state_mutex);
		time_since_meal = current_time - philo->philosophers[i].last_meal_time;
		pthread_mutex_unlock(&philo->state_mutex);
		if (time_since_meal > philo->time_to_die)
		{
			pthread_mutex_lock(&philo->state_mutex);
			philo->sim_state = SOMEONE_DIED;
			philo->philosophers[i].state |= DEAD;
			pthread_mutex_unlock(&philo->state_mutex);
			print_death(&philo->philosophers[i]);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	check_completion(t_philo_system *philo)
{
	int	i;
	int	satisfied_count;

	if (philo->target_meal_count == -1)
		return (false);
	satisfied_count = 0;
	i = 0;
	while (i < philo->nb_philos)
	{
		pthread_mutex_lock(&philo->state_mutex);
		if (philo->philosophers[i].meal_count >= philo->target_meal_count)
			satisfied_count++;
		pthread_mutex_unlock(&philo->state_mutex);
		i++;
	}
	if (satisfied_count == philo->nb_philos)
	{
		pthread_mutex_lock(&philo->state_mutex);
		philo->sim_state = ALL_SATISFIED;
		pthread_mutex_unlock(&philo->state_mutex);
		return (true);
	}
	return (false);
}
