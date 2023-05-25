/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:14:36 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/24 20:26:02 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	ft_sleep(t_params *params, unsigned long long time)
{
	unsigned long long	limit;

	limit = get_current_ts() + time;
	while (get_current_ts() < limit)
	{
		if (is_stopped(params))
			break;
		usleep(100);
	}
}

void	*alone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->forks[philo->right_fork]);
	print_action(philo, "has taken a fork");
	ft_sleep(philo->params, philo->params->time_to_die);
	print_action(philo, "died");
	pthread_mutex_unlock(&philo->params->forks[philo->right_fork]);
	return (NULL);
}

void	ft_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->params->forks[philo->left_fork]);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->params->forks[philo->right_fork]);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->params->forks[philo->right_fork]);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->params->forks[philo->left_fork]);
		print_action(philo, "has taken a fork");
	}
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_ts = get_current_ts();
	pthread_mutex_unlock(&philo->last_meal_lock);
	ft_sleep(philo->params, philo->params->time_to_eat);
	if (!is_stopped(philo->params))
	{
		pthread_mutex_lock(&philo->last_meal_lock);
		philo->meals_nb += 1;
		pthread_mutex_unlock(&philo->last_meal_lock);
	}
	print_action(philo, "is sleeping");
	pthread_mutex_unlock(&philo->params->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->params->forks[philo->left_fork]);
	ft_sleep(philo->params, philo->params->time_to_sleep);
}

void	ft_think(t_philo *philo)
{
	print_action(philo, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("Hello, I'm philo %d\n", philo->id);

	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal_ts = philo->params->start_ts;
	pthread_mutex_unlock(&philo->last_meal_lock);
	if (philo->params->nb_philos == 1)
		return (alone_philo_routine(philo));
	while (!is_stopped(philo->params))
	{
		ft_eat(philo);
		ft_think(philo);
	}

	return (NULL);
}