/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:32:11 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/28 12:13:18 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(t_philo_system *s, long long milliseconds)
{
	long long	start;
	long long	current;

	start = get_time();
	while (1)
	{
		current = get_time();
		if (!(s->sim_state == RUNNING) || current - start >= milliseconds)
			break ;
		usleep(50);
	}
}
