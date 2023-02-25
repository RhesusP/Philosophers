/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:04:54 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/25 17:59:17 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Initialize fields of a single philosophers.
 * 
 * @param philo Pointer to a t_philo. 
 * @param id Philosopher identifier. 
 * @param param Pointer to t_params.
 * @return 1 if mutex properly initialized. 0 if not.
 */
int	init_philo(t_philo *philo, int id, t_params *param)
{
	philo->id = id + 1;
	philo->last_meal_ts = 0;
	if (pthread_mutex_init(&philo->last_meal_lock, NULL))
		return (0);
	philo->nb_meal = 0;
	philo->params = param;
	philo->left_fork = &param->forks[id];
	if (id - 1 < 0)
		philo->right_fork = &param->forks[param->nb_philos - 1];
	else
		philo->right_fork = &param->forks[id - 1];
	return (1);
}

/**
 * @brief Initialize a t_philo array thanks to param rules.
 * 
 * @param philos Pointer to t_philo array.
 * @param param Pointer to t_params
 * @return 1 of evrything is properly allocated. 0 if not.
 */
int	create_philos_array(t_philo **philos, t_params *param)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * param->nb_philos);
	if (!*philos)
		return (0);
	i = 0;
	while (i < param->nb_philos)
	{
		if (!init_philo(&(*philos)[i], i, param))
		{
			free(param->forks);
			free(*philos);
			return (0);
		}
		i++;
	}
	return (1);
}
