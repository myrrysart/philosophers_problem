/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:05:36 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/23 07:18:13 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILOS 300

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
}								t_sim_state;

typedef struct s_philo_system	t_philo_system;

typedef struct s_philosopher
{
	t_philo_system				*system;
	int							id;
	pthread_t					thread;
	long long					last_meal_time;
	long long					next_deadline_ms;
	int							meal_count;
	unsigned int				state;
	int							left_fork_id;
	int							right_fork_id;

	int							frst_fork_id;
	int							scnd_fork_id;
	pthread_mutex_t				*frst_fork;
	pthread_mutex_t				*scnd_fork;
	int							is_odd;
	long long					stagger_ms;
	int							jitter_us;
	int							satisfied_marked;

	pthread_mutex_t				lock;
}								t_philosopher;

typedef struct s_philo_system
{
	int							nb_philos;
	long long					time_to_die;
	long long					time_to_eat;
	long long					time_to_sleep;
	long long					eat_half;
	long long					die_minus_eat;
	int							target_meal_count;
	long long					start_time;
	int							satisfied_count;

	t_philosopher				philosophers[MAX_PHILOS];
	pthread_t					monitor_thread;

	pthread_mutex_t				forks[MAX_PHILOS];

	unsigned int				sim_state;
	pthread_mutex_t				state_mutex;
	pthread_mutex_t				output_mutex;
}								t_philo_system;

// Core system functions
int								main(int argc, char **argv);
int								init_system(t_philo_system *philo, char **argv,
									int argc);
void							cleanup_system(t_philo_system *philo);
int								init_mutexes(t_philo_system *philo);

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
void							update_meal_data(t_philosopher *philosopher);

// System monitors
bool							check_deaths(t_philo_system *philo);
bool							check_completion(t_philo_system *philo);

// Utilities
long long						get_time(void);
void							precise_sleep(long long milliseconds);
void							print_action(t_philosopher *philosopher,
									char *action);
void							print_death(t_philosopher *philosopher);
int								safe_atoi(char *str);
int								init_mutexes(t_philo_system *philo);

#endif
