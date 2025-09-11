#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>
# include <semaphore.h>
# include <fcntl.h>

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	long long		start_time;
	sem_t			*forks;
	sem_t			*write_lock;
	sem_t			*dead_lock;
	pid_t			*pids;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	t_data			*data;
}	t_philo;

int			main(int argc, char **argv);
int			check_args(int argc, char **argv);
int			ft_atoi(const char *str);
int			init_data(t_data *data, char **argv, int argc);
int			init_semaphores(t_data *data);
void		philo_process(t_philo *philo);
void		*monitor(void *arg);
long long	get_time(void);
void		ft_usleep(long long milliseconds);
void		print_message(char *str, t_philo *philo);
void		cleanup(t_data *data);
void		kill_all_processes(t_data *data);

#endif