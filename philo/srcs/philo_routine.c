/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:14:36 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/23 12:59:16 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Philo routine when there is only one philosopher.
 * The lonely philosopher takes the right fork, then dies.
 * @param arg Philo pointer casted as void*
 * @return null
 */
static void	*solo_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->param->forks[philo->right_fork]);
	print_action(philo, "has taken a fork 🍴");
	ft_sleep(philo->param, philo->param->time_to_die);
	print_action(philo, "died 💀");
	pthread_mutex_unlock(&philo->param->forks[philo->right_fork]);
	return (NULL);
}

/**
 * @brief Synchronize threads to start at the same time.
 * 
 * @param time time to start at.
 */
void	synchronize_threads(unsigned long long time)
{
	while (get_current_ts() < time)
		continue ;
}

/**
 * @brief Philo think routine.
 * 
 * @param philo
 */
static void	ft_think(t_philo *philo)
{
	print_action(philo, "is thinking 💭");
}

/**
 * @brief Philo eat and sleep routine.
 * Philo takes the forks, eat during time_to_eat, then release the
 * forks and sleep during time_to_sleep.
 * 
 * @param philo
 */
static void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->param->forks[philo->left_fork]);
	print_action(philo, "has taken a fork 🍴");
	pthread_mutex_lock(&philo->param->forks[philo->right_fork]);
	print_action(philo, "has taken a fork 🍴");
	print_action(philo, "is eating 🍝");
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
	print_action(philo, "is sleeping 🛌");
	pthread_mutex_unlock(&philo->param->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->param->forks[philo->left_fork]);
	ft_sleep(philo->param, philo->param->time_to_sleep);
}

/**
 * @brief Philo routine
 * Eat, sleep and think, this is the simple life of a philosopher.
 * @param arg t_philo pointer casted as void*
 * @return null
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	synchronize_threads(philo->param->start_time);
	if (philo->param->time_to_die == 0)
		return (NULL);
	if (philo->param->nb_philos == 1)
		return (solo_philo_routine(philo));
	if (philo->id % 2 == 0)
		ft_sleep(philo->param, philo->param->time_to_eat);
	while (!is_stopped(philo->param))
	{
		ft_eat(philo);
		ft_think(philo);
	}
	return (NULL);
}
