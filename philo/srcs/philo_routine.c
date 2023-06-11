/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:14:36 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/11 12:50:52 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	*solo_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->param->forks[philo->right_fork]);
	print_action(philo, "has taken a fork");
	ft_sleep(philo->param, philo->param->time_to_die);
	print_action(philo, "died");
	pthread_mutex_unlock(&philo->param->forks[philo->right_fork]);
	return (NULL);
}

void	ft_think(t_philo *philo)
{
	//ft_sleep(philo->param, philo->param.)
	print_action(philo, "is thinking");
}

void	synchronize_threads(unsigned long long time)
{
	while (get_current_ts() < time)
		continue ;
}

void	ft_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->param->forks[philo->left_fork]);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->param->forks[philo->right_fork]);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->param->forks[philo->right_fork]);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->param->forks[philo->left_fork]);
		print_action(philo, "has taken a fork");
	}
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_ts = get_current_ts();
	pthread_mutex_unlock(&philo->last_meal_lock);
	ft_sleep(philo->param, philo->param->time_to_eat);
	if (!is_stopped(philo->param))
	{
		pthread_mutex_lock(&philo->last_meal_lock);
		philo->nb_meals++;
		pthread_mutex_unlock(&philo->last_meal_lock);
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->param->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->param->forks[philo->left_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->param->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->param->forks[philo->right_fork]);
	}
	print_action(philo, "is sleeping");
	ft_sleep(philo->param, philo->param->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	synchronize_threads(philo->param->start_time);
	if (philo->param->time_to_die == 0)
		return (NULL);
	if (philo->param->nb_philos == 1)
		return (solo_philo_routine(philo));
	while (!is_stopped(philo->param))
	{
		ft_eat(philo);
		ft_think(philo);
	}

	return (NULL);
}
