/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:39:16 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/27 13:44:06 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	take_forks(t_philo *philo)
{
	if (philo->params->nb_philos == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		ft_usleep(philo->params->time_to_die);
		return (1);
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	print_action(philo, "is eating");
	return (0);
}

void	ft_sleep(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->params->time_to_sleep);
}

void	ft_think(t_philo *philo)
{
	print_action(philo, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!is_philo_dead(philo->params))
	{
		if (take_forks(philo) == 1)
			break ;
		pthread_mutex_lock(&(philo->last_meal_lock));
		philo->last_meal_ts = get_current_ts();
		pthread_mutex_unlock(&(philo->last_meal_lock));
		ft_usleep(philo->params->time_to_eat);
		pthread_mutex_lock(&(philo->nb_meal_lock));
		philo->nb_meal++;
		pthread_mutex_unlock(&(philo->nb_meal_lock));
		if (philo->params->nb_philos > 1)
			pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (did_all_philos_eat(philo->params))
			break ;
		ft_sleep(philo);
		ft_think(philo);
	}
	return (0);
}
