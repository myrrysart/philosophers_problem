/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:20:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/12 19:20:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*monitor_routine(void *arg)
{
	t_philo_system	*philo;

	philo = (t_philo_system *)arg;
	while (1)
	{
		if (check_deaths(philo))
			break ;
		if (check_completion(philo))
			break ;
		usleep(100);
	}
	return (NULL);
}
