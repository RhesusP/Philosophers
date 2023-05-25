/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:39:24 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/25 16:45:32 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	is_stopped(t_params *params)
{
	int	stopped;

	pthread_mutex_lock(&params->stop_lock);
	stopped = params->stop;
	pthread_mutex_unlock(&params->stop_lock);
	return (stopped);
}

static int	set_stop(t_params *params, int value)
{
	pthread_mutex_lock(&params->stop_lock);
	params->stop = value;
	pthread_mutex_unlock(&params->stop_lock);
	return (1);
}

static int	need_stop(t_params *params)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < params->nb_philos)
	{
		pthread_mutex_lock(&params->philo_tab[i].last_meal_lock);
		if (is_philo_dead(&params->philo_tab[i]))
		{
			pthread_mutex_unlock(&params->philo_tab[i].last_meal_lock);
			return (1);
		}
		if (params->max_meal != -1)
		{
			if (params->philo_tab[i].meals_nb < params->max_meal)
				all_ate = 0;
		}
		pthread_mutex_unlock(&params->philo_tab[i].last_meal_lock);
		i++;
	}
	if (params->max_meal != -1 && all_ate)
		return (set_stop(params, 1));
	return (0);
}

void	*death_routine(void *arg)
{
	t_params	*params;

	params = (t_params *)arg;
	usleep(10);
	while (1)
	{
		if (need_stop(params))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
