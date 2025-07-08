/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:32:11 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/24 11:32:13 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long milliseconds)
{
	long long	start;
	long long	current;

	start = get_time();
	while (1)
	{
		current = get_time();
		if (current - start >= milliseconds)
			break ;
		usleep(500);
	}
}

void	ft_usleep_check(long long milliseconds, t_data *data)
{
	long long	start;
	long long	current;

	start = get_time();
	while (1)
	{
		current = get_time();
		if (current - start >= milliseconds)
			break ;
		if (!should_continue(data))
			break ;
		usleep(500);
	}
}
