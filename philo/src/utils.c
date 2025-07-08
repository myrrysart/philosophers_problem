/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:32:13 by jyniemit          #+#    #+#             */
/*   Updated: 2025/06/24 11:32:14 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_atoi_strict(char *str)
{
	unsigned long	result;
	int				i;

	if (!str || *str == '\0')
		return (-1);
	i = 0;
	result = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (-1);
		result = result * 10 + (str[i] - '0');
		if (result > UINT_MAX)
			return (-1);
		i++;
	}
	if (result == 0)
		return (-1);
	return ((int)result);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	value;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Invalid number of arguments.\n");
		printf("Usage: %s number_of_philosophers time_to_die ", argv[0]);
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		value = ft_atoi_strict(argv[i]);
		if (value == -1)
		{
			printf("Error: Invalid argument '%s'. ", argv[i]);
			printf("All arguments must be positive integers.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	return (ft_atoi_strict((char *)str));
}

int	should_continue(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_lock);
	result = !data->someone_dead && !data->meals_finished;
	pthread_mutex_unlock(&data->death_lock);
	return (result);
}
