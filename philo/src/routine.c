/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:32:09 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/24 11:32:10 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	grab_forks(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(&philo->data->forks[first_fork]);
	print_message("has taken a fork", philo);
	pthread_mutex_lock(&philo->data->forks[second_fork]);
	print_message("has taken a fork", philo);
}

static void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}

static void	eat(t_philo *philo)
{
	print_message("is eating", philo);
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_lock);
	ft_usleep_check(philo->data->time_to_eat, philo->data);
}

void	*philo_routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	if (philo->data->nb_of_philo == 1)
	{
		print_message("is thinking", philo);
		print_message("has taken a fork", philo);
		ft_usleep(philo->data->time_to_die + 1);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep_check(philo->data->time_to_eat / 2, philo->data);
	while (should_continue(philo->data))
	{
		print_message("is thinking", philo);
		grab_forks(philo);
		eat(philo);
		release_forks(philo);
		print_message("is sleeping", philo);
		ft_usleep_check(philo->data->time_to_sleep, philo->data);
	}
	return (NULL);
}
