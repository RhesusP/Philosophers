/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 18:34:45 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/22 18:48:41 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo_bonus.h"

t_philo	*init_philo(t_philo *philo, int id, t_params *params)
{
	philo->id = id;
	philo->last_meal_ts = 0;
	philo->nb_meal = 0;
	philo->params = params;
	return (philo);
}


int	create_philos_array(t_philo **philos, t_params *param)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * param->nb_philos);
	if (!*philos)
		return (0);
	i = 0;
	while (i < param->nb_philos)
	{
		if (!init_philo(&(*philos)[i], i + 1, param))
		{
			//free(param->forks);
			free(*philos);
			return (0);
		}
		i++;
	}
	return (1);
}