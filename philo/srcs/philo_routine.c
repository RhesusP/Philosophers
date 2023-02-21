/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:39:16 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/21 22:29:08 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Take left and right forks. If the philosopher's id is even, then the 
 * philosopher takes the left fork then the right fork. If the id is odd, it does
 * the opposite.
 * 
 * @param philo Pointer to a philosopher.
 */
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

/**
 * @brief Philosopher printing its action and sleep for a time.
 * 
 * @param philo Pointer to a philosopher.
 */
void	ft_sleep(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->params->time_to_sleep);
}

/**
 * @brief a very complicated function that allows the philosopher to think.
 * 
 * @param philo Pointer to a philosopher.
 */
void	ft_think(t_philo *philo)
{
	print_action(philo, "is thinking");
}

/**
 * @brief The philosopher's routine. Eat, sleep and think until he's dead. It 
 * stops if the t_params's variable "is_dead" is true or if the philosophers
 * ate more than max_meal (t_param's variable) times.
 * 
 * @param arg A pointer to a philosopher.
 * @return NULL.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		max_meal;

	philo = (t_philo *)arg;
	max_meal = philo->params->max_meal;
	while (!is_philo_dead(philo))
	{
		if (max_meal >= 0 && philo->nb_meal >= max_meal)
			break ;
		take_forks(philo);
		philo->last_meal_ts = get_current_ts();
		print_action(philo, "\033[95mis eating\033[39m");
		ft_usleep(philo->params->time_to_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		philo->nb_meal++;
		ft_sleep(philo);
		ft_think(philo);
	}
	return (0);
}
