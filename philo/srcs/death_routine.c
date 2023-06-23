/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:39:24 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/23 13:03:18 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Check if a single philosopher died.
 * 
 * @param philo Philosopher to check
 * @return 1 if the philosopher died, 0 otherwise
 */
static int	philo_died(t_philo *philo)
{
	unsigned long long	now;
	unsigned long long	time_to_die;

	time_to_die = (unsigned long long)philo->param->time_to_die;
	now = get_current_ts();
	if (now - philo->last_meal_ts >= time_to_die)
	{
		pthread_mutex_lock(&philo->param->is_dead_lock);
		philo->param->is_dead = 1;
		pthread_mutex_unlock(&philo->param->is_dead_lock);
		print_action(philo, "died 💀");
		pthread_mutex_unlock(&philo->last_meal_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Stopper if all philosophers ate.
 * If all philosophers ate, set is_dead to 1.
 * @param param 
 * @param all_ate 
 * @return all_ate
 */
static int	all_philos_ate(t_params *param, int all_ate)
{
	if (param->max_meal != -1 && all_ate)
	{
		pthread_mutex_lock(&param->is_dead_lock);
		param->is_dead = 1;
		pthread_mutex_unlock(&param->is_dead_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Check simulation stop conditions (philosopher died or all ate)
 * @param param 
 * @return 1 if stop conditions are met, 0 otherwise
 */
static int	need_stop(t_params *param)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < param->nb_philos)
	{
		pthread_mutex_lock(&param->philos[i].last_meal_lock);
		if (philo_died(&param->philos[i]))
			return (1);
		if (param->max_meal != -1)
		{
			if (param->philos[i].nb_meals < param->max_meal)
				all_ate = 0;
		}
		pthread_mutex_unlock(&param->philos[i].last_meal_lock);
		i++;
	}
	if (all_philos_ate(param, all_ate))
		return (1);
	return (0);
}

/**
 * @brief Death checker routine.
 * Check if a philosopher died or if all philosophers ate.
 * @param arg t_params pointer casted as void*
 * @return null
 */
void	*death_routine(void *arg)
{
	t_params	*param;

	param = (t_params *)arg;
	if (param->max_meal == 0)
		return (NULL);
	synchronize_threads(param->start_time);
	pthread_mutex_lock(&param->is_dead_lock);
	param->is_dead = 0;
	pthread_mutex_unlock(&param->is_dead_lock);
	while (1)
	{
		if (need_stop(param))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
