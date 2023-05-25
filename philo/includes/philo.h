/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:23:06 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/25 16:49:51 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

struct	s_philo;

typedef struct s_params
{
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_meal;
	int					all_ate;
	unsigned long long	start_ts;
	int					stop;
	pthread_mutex_t		stop_lock;
	pthread_mutex_t		write_lock;
	pthread_t			death_checker;
	pthread_mutex_t		*forks;
	struct s_philo		*philo_tab;

}	t_params;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					meals_nb;
	int					left_fork;
	int					right_fork;
	unsigned long long	last_meal_ts;
	pthread_mutex_t		last_meal_lock;
	t_params			*params;
}	t_philo;

void				*death_routine(void *arg);
int					ft_atoi(const char *str);
int					print_error(char *msg);
int					init_params(t_params *param, int argc, char **argv);
int					is_input_valid(t_params *param, char **argv, int argc);
void				*philo_routine(void *arg);
int					is_stopped(t_params *params);

unsigned long long	get_current_ts(void);
void				ft_sleep(t_params *params, unsigned long long time);
void				print_action(t_philo *philo, char *action);
int					is_philo_dead(t_philo *philo);

#endif