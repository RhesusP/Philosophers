/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:23:06 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/14 11:44:10 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

struct	s_params;

typedef struct s_philo
{
	pthread_t			thread;				
	int					id;
	int					nb_meals;
	unsigned long long	last_meal_ts;
	pthread_mutex_t		last_meal_lock;
	int					left_fork;
	int					right_fork;
	struct s_params		*param;	
}	t_philo;

typedef struct s_params
{
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_meal;
	unsigned long long	start_time;
	int					is_dead;
	pthread_mutex_t		is_dead_lock;
	t_philo				*philos;		
	pthread_t			death_checker;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_lock;
}	t_params;

int					ft_atoi(const char *str);
int					print_error(char *msg);
t_params			*init_params(int argc, char **argv);
int					is_input_valid(t_params *param, char **argv, int argc);
void				*philo_routine(void *arg);
void				*death_routine(void *arg);

void				print_action(t_philo *philo, char *status, int force);
void				ft_sleep(t_params *params, unsigned long long time);
int					is_stopped(t_params *param);
unsigned long long	get_current_ts(void);
void				synchronize_threads(unsigned long long time);

#endif