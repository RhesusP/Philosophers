/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:39:16 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/12 02:19:56 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	take_forks(t_philo *philo)
{
	int		is_dead;

	pthread_mutex_lock(&(philo->params->is_dead_lock));
	is_dead = philo->params->is_dead;
	pthread_mutex_unlock(&(philo->params->is_dead_lock));
	if (is_dead)
		return ;
	if (philo->id % 2 == 0)
	{
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
	}
	else
	{
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
	}
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
	t_philo *philo;
	unsigned long long	last_meal;

	philo = (t_philo *)arg;
	while (!is_philo_dead(philo))
	{
		if (philo->params->max_meal >= 0 && philo->nb_meal >= philo->params->max_meal)
			break;

		take_forks(philo);
		philo->last_meal_ts = get_current_ts();
		print_action(philo, "is eating");
		ft_usleep(philo->params->time_to_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		philo->nb_meal++;
		ft_sleep(philo);
		ft_think(philo);
	}
	return (0);
}




int	death_checker(t_philo *philo, unsigned long long current_ms)
{
	unsigned long long	last_meal_ts;
	
	pthread_mutex_lock(&philo->last_meal_lock);
	last_meal_ts = current_ms - philo->last_meal_ts;
	pthread_mutex_unlock(&philo->last_meal_lock);
	if (last_meal_ts > philo->params->time_to_die)
	{
		pthread_mutex_lock(&philo->params->write_lock);
		pthread_mutex_lock(&philo->params->is_dead_lock);
		philo->params->is_dead = 1;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->params->is_dead_lock);
		printf("\033[31m%lld %d died\033[39m\n", last_meal_ts, philo->id);
		pthread_mutex_unlock(&philo->params->write_lock);
		return (1);
	}
	return (0);
}

void	*death_checker_routine(void *arg)
{
	t_philo		**philos;
	t_params	*params;
	int			i;
	unsigned long long	current_time;

	philos = (t_philo **)arg;
	params = philos[i]->params;
	while (1)
	{
		i = 0;
		current_time = get_current_ts() - params->start_ts;
		while (i < params->nb_philos)
		{
			if (death_checker(philos[i], get_current_ts()))
				return (0);
		}
		ft_usleep(1);
	}
	return (0);
}