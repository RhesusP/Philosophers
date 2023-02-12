/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:23:06 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/12 17:26:32 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_params
{
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_meal;
	unsigned long long	start_ts;
	int					is_dead;
	pthread_mutex_t		is_dead_lock;
	pthread_t			death_checker;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*forks;
}	t_params;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	unsigned long long	last_meal_ts;
	pthread_mutex_t		last_meal_lock;
	int					nb_meal;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	t_params			*params;
}	t_philo;

int					ft_atoi(const char *str);
unsigned long long	get_current_ts(void);
void				ft_usleep(unsigned long long ms_duration);
void				print_action(t_philo *philo, char *action);
int					is_philo_dead(t_philo *philo);

int					init_params(t_params *param, int argc, char **argv);
int					create_philos_array(t_philo **philos, t_params *param);
void				launch_threads(t_params *params, t_philo **philos);

void				*philo_routine(void *arg);
void				*death_checker_routine(void *arg);

void				free_stuff(t_philo **philos, t_params *param);

#endif