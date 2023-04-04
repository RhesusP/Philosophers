/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker_routine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:09:43 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/04 23:08:02 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	*death_checker_routine(void *arg)
{
	t_params	*params;
	int			i;
	int			nb_meal;
	unsigned long long	last_meal_ts;

	params = (t_params *)arg;
	while (!did_all_philos_eat(params))
	{
		i = 0;
		while (i < params->nb_philos && !is_philo_dead(params))
		{
			pthread_mutex_lock(&(params->philo_tab[i].last_meal_lock));
			last_meal_ts = get_current_ts() - params->philo_tab[i].last_meal_ts;
			pthread_mutex_unlock(&(params->philo_tab[i].last_meal_lock));
			if (last_meal_ts > (unsigned long long)params->time_to_die)
			{
				print_action(&(params->philo_tab[i]), "is dead");
				pthread_mutex_lock(&params->is_dead_lock);
				params->is_dead = 1;
				pthread_mutex_unlock(&params->is_dead_lock);
			}	
			i++;
			ft_usleep(1);
		}
		if (is_philo_dead(params))
			break;
		i = 0;
		pthread_mutex_lock(&(params->philo_tab[i].nb_meal_lock));
		nb_meal = params->philo_tab[i].nb_meal;
		pthread_mutex_unlock(&(params->philo_tab[i].nb_meal_lock));
		while (params->max_meal != -1 && i < params->nb_philos && nb_meal >= params->max_meal)
			i++;
		if (i == params->nb_philos)
		{
			pthread_mutex_lock(&(params->all_eat_lock));
			params->all_ate = 1;
			pthread_mutex_unlock(&(params->all_eat_lock));
		}
	}
	return (0);
}
