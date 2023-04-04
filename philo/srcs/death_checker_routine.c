/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker_routine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:09:43 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/04 14:43:55 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	*death_checker_routine(void *arg)
{
	t_params	*params;
	int			i;
	unsigned long long	last_meal_ts;

	params = (t_params *)arg;
	while (!params->all_ate)
	{
		i = 0;
		while (i < params->nb_philos && !params->is_dead)
		{
			pthread_mutex_lock(&(params->philo_tab[i].last_meal_lock));
			last_meal_ts = get_current_ts() - params->philo_tab[i].last_meal_ts;
			pthread_mutex_unlock(&(params->philo_tab[i].last_meal_lock));
			if (last_meal_ts > (unsigned long long)params->time_to_die)
			{
				print_action(&(params->philo_tab[i]), "is dead");
				pthread_mutex_lock(&params->is_dead_lock);
				params->is_dead = 1;
				pthread_mutex_unlock(params->philo_tab[i].left_fork);
				pthread_mutex_unlock(params->philo_tab[i].right_fork);
				pthread_mutex_unlock(&params->is_dead_lock);
			}
			i++;
			ft_usleep(1);
		}
		if (params->is_dead)
			break;
		i = 0;
		while (params->max_meal != -1 && i < params->nb_philos && params->philo_tab[i].nb_meal >= params->max_meal)
			i++;
		if (i == params->nb_philos)
			params->all_ate = 1;
	}
	return (0);
}
