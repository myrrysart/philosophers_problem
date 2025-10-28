/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:05:36 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/28 11:38:51 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdatomic.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>

# define NORMAL_EXIT -1

typedef enum e_philo_state
{
	DEAD = (1 << 0),
	SATISFIED = (1 << 1),
}								t_philo_state;

typedef enum e_sim_state
{
	RUNNING = 0,
	SOMEONE_DIED = (1 << 0),
	ALL_SATISFIED = (1 << 1),
	PHILO_ERROR = (1 << 2),
	MALLOC_ERROR = (1 << 3),
	MUTEX_ERROR = (1 << 4),
	WAITING = (1 << 5),
}								t_sim_state;

typedef struct s_philo_system	t_philo_system;

typedef struct s_parsed_args
{
	int							nb_philos;
	long long					time_to_die;
	long long					time_to_eat;
	long long					time_to_sleep;
	int							target_meal_count;
}								t_parsed_args;

typedef struct s_philosopher
{
	t_philo_system				*system;
	int							id;
	int							nb_philos;
	pthread_t					thread;

	_Atomic long long			start_time;
	long long					time_to_die;
	long long					time_to_eat;
	long long					time_to_sleep;
	int							target_meal_count;

	_Atomic long long			last_meal_time;
	_Atomic long long			next_deadline_ms;
	_Atomic int					meal_count;
	atomic_int					state;

	pthread_mutex_t				*frst_fork;
	pthread_mutex_t				*scnd_fork;
	_Atomic long long			stagger_ms;
}								t_philosopher;

typedef struct s_philo_system
{
	int							nb_philos;
	_Atomic long long			start_time;
	atomic_int					satisfied_count;

	t_philosopher				*philosophers;
	int							threads_initiated;
	pthread_t					monitor_thread;
	int							monitor_initiated;

	pthread_mutex_t				*forks;
	int							mutex_initiated;

	atomic_int					sim_state;
	pthread_mutex_t				output_mutex;
	int							output_mutex_initiated;
}								t_philo_system;

// Core system functions
int								main(int argc, char **argv);
int								init_system(t_philo_system *sim, char **argv,
									int argc);
int								cleanup_system(t_philo_system *sim);
int								init_mutexes(t_philo_system *sim);

// Thread functions
void							*philo_routine(void *arg);
void							*monitor_routine(void *arg);
void							philo_eat(t_philosopher *philosopher);
void							philo_sleep(t_philosopher *philosopher);
void							philo_think(t_philosopher *philosopher);
bool							should_continue(t_philosopher *p);

// Core operations
bool							try_acquire_forks(t_philosopher *philosopher);
void							release_forks(t_philosopher *philosopher);

// Utilities
long long						get_time(void);
void							precise_sleep(t_philo_system *s,
									long long milliseconds);
void							print_action(t_philosopher *philosopher,
									char *action);
void							print_death(t_philosopher *philosopher);
int								safe_atoi(char *str);

#endif
