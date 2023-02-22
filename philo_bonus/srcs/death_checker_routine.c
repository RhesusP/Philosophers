/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker_routine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 18:01:25 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/22 18:46:03 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo_bonus.h"

/**
 * @brief Checks that a philosopher does not starve. If it does, the t_param's
 * variable "is_dead" is set to true.
 * 
 * @param philo A pointer to a philosopher.
 * @param current_ms Current time in millisecondes.
 * @return 1 if the philosopher died. O if he's still alive.
 */
int	death_checker(t_philo *philo, unsigned long long current_ms)
{
	unsigned long long	last_meal_ts;

	sem_wait(philo->last_meal_lock);
	last_meal_ts = current_ms - philo->last_meal_ts;
	sem_post(philo->last_meal_lock);
	if (last_meal_ts > (unsigned long long)philo->params->time_to_die)
	{
		sem_wait(philo->params->write_lock);
		sem_wait(philo->params->is_dead_lock);
		philo->params->is_dead = 1;
		sem_post(philo->params->forks);
		sem_post(philo->params->forks);
		sem_post(philo->params->is_dead_lock);
		printf("\033[31m%lld %d died\033[39m\n", last_meal_ts, philo->id);
		sem_post(philo->params->write_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Death checker's routine that checks if each philosopher respects 
 * the rules defined in the t_params.
 * 
 * @param arg A pointer to a t_philo array (which contains a pointer 
 * to t_params).
 * @return NULL.
 */
void	*death_checker_routine(void *arg)
{
	t_philo		**philos;
	t_params	*params;
	int			i;

	philos = (t_philo **)arg;
	params = philos[0]->params;
	while (1)
	{
		i = 0;
		while (i < params->nb_philos)
		{
			if (death_checker(philos[i], get_current_ts()))
				return (0);
		}
		ft_usleep(1);
	}
	return (0);
}
