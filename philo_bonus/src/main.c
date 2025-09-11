#include "../includes/philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	int			i;

	if (check_args(argc, argv) == 1)
		return (1);
	if (init_data(&data, argv, argc) == 1)
		return (1);
	philos = malloc(sizeof(t_philo) * data.nb_philo);
	if (!philos)
		return (1);
	i = 0;
	while (i < data.nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = get_time();
		philos[i].data = &data;
		data.pids[i] = fork();
		if (data.pids[i] == 0)
		{
			philo_process(&philos[i]);
			exit(0);
		}
		i++;
	}
	i = 0;
	while (i < data.nb_philo)
	{
		waitpid(data.pids[i], NULL, 0);
		i++;
	}
	cleanup(&data);
	free(philos);
	return (0);
}