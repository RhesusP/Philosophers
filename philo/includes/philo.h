/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:23:06 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/05 18:29:23 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct	s_philo
{
	int				id;
	int				is_alive;
	pthread_mutex_t	*life_lock;
	unsigned long long	birth;
	unsigned long long	last_meal;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	int				*is_first;
	pthread_mutex_t	*first_lock;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
}	t_philo;

typedef struct	s_params
{
	int				nb_philos;
	int				died;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				is_first;
	pthread_mutex_t	*first_lock;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	t_philo			**philos;
}	t_params;

int	ft_atoi(const char *str);
void	*thread_routine(void *arg);

#endif