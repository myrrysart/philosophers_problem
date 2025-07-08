/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:32:01 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/24 11:32:03 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	check_meal_completion(t_philo *philos)
{
	int	i;
	int	all_ate_enough;

	if (philos[0].data->nb_times_to_eat == -1)
		return (0);
	pthread_mutex_lock(&philos[0].data->death_lock);
	all_ate_enough = 1;
	i = 0;
	while (i < philos[0].data->nb_of_philo)
	{
		if (philos[i].meals_eaten < philos[0].data->nb_times_to_eat)
		{
			all_ate_enough = 0;
			break ;
		}
		i++;
	}
	if (all_ate_enough)
		philos[0].data->meals_finished = 1;
	pthread_mutex_unlock(&philos[0].data->death_lock);
	return (all_ate_enough);
}

static int	check_philosopher_death(t_philo *philos, int i)
{
	long long	last_meal;
	long long	current_time;

	pthread_mutex_lock(&philos[0].data->death_lock);
	if (philos[0].data->someone_dead)
	{
		pthread_mutex_unlock(&philos[0].data->death_lock);
		return (0);
	}
	last_meal = philos[i].last_meal;
	current_time = get_time();
	if (current_time - last_meal > philos[0].data->time_to_die)
	{
		pthread_mutex_unlock(&philos[0].data->death_lock);
		pthread_mutex_lock(&philos[0].data->write_lock);
		pthread_mutex_lock(&philos[0].data->death_lock);
		philos[0].data->someone_dead = 1;
		pthread_mutex_unlock(&philos[0].data->death_lock);
		printf("%lld %d died\n", current_time - philos[0].data->start_time,
			philos[i].id);
		pthread_mutex_unlock(&philos[0].data->write_lock);
		return (1);
	}
	pthread_mutex_unlock(&philos[0].data->death_lock);
	return (0);
}

static int	check_deaths_batch(t_philo *philos, int start, int end)
{
	int	i;

	i = start;
	while (i < end && i < philos[0].data->nb_of_philo)
	{
		if (check_philosopher_death(philos, i))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_philo	*philos;
	int		cycle;
	int		chunk;

	philos = (t_philo *)arg;
	cycle = 0;
	while (should_continue(philos[0].data))
	{
		if (cycle % 10 == 0 && check_meal_completion(philos))
			return (NULL);
		chunk = 0;
		while (chunk < philos[0].data->nb_of_philo)
		{
			if (check_deaths_batch(philos, chunk, chunk + 32))
				return (NULL);
			chunk += 32;
			if (chunk < philos[0].data->nb_of_philo)
				usleep(10);
		}
		cycle++;
		usleep(1000);
	}
	return (NULL);
}
