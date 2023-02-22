/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:23:06 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/22 18:47:21 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>  
# include <sys/time.h>
# include <pthread.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>  
# include <semaphore.h>

typedef struct s_params
{
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_meal;
	unsigned long long	start_ts;
	pid_t				*pids;
	sem_t				*forks;
	sem_t				*write_lock;
	sem_t				*is_dead_lock;
	int					is_dead;
	pthread_t			death_checker;
}	t_params;

typedef struct s_philo
{
	int					id;
	unsigned long long	last_meal_ts;
	sem_t				*last_meal_lock;
	int					nb_meal;
	t_params			*params;
}	t_philo;

int		ft_atoi(const char *str);
void	philo_routine(t_philo *philo);

unsigned long long	get_current_ts(void);
void	ft_usleep(unsigned long long ms_duration);
void	print_action(t_philo *philo, char *action);
int		is_philo_dead(t_philo *philo);

int	create_philos_array(t_philo **philos, t_params *param);
void	*death_checker_routine(void *arg);

#endif