/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:32:06 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/24 11:32:08 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_message(char *str, t_philo *philo)
{
	long long	timestamp;
	int			should_print;

	pthread_mutex_lock(&philo->data->write_lock);
	pthread_mutex_lock(&philo->data->death_lock);
	should_print = !philo->data->someone_dead && !philo->data->meals_finished;
	pthread_mutex_unlock(&philo->data->death_lock);
	if (should_print)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, str);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}
