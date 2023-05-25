/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:39:24 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/25 12:03:03 by cbernot          ###   ########.fr       */
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

void	print_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->params->write_lock);
	if (is_stopped(philo->params))
	{
		pthread_mutex_unlock(&philo->params->write_lock);
		return ;
	}
	printf("%lld %d %s\n", get_current_ts() - philo->params->start_ts, philo->id, action);
	pthread_mutex_unlock(&philo->params->write_lock);
}

int	is_philo_dead(t_philo *philo)
{
	if (get_current_ts() - philo->last_meal_ts >= philo->params->time_to_die)
	{
		pthread_mutex_lock(&philo->params->stop_lock);
		philo->params->stop = 1;
		pthread_mutex_unlock(&philo->params->stop_lock);
		print_action(philo, "died");
		return (1);
	}
	return (0);
}

int	need_stop(t_params *params)
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
	{
		pthread_mutex_lock(&params->stop_lock);
		params->stop = 1;
		pthread_mutex_unlock(&params->stop_lock);
		return (1);
	}
	return (0);
}

void	*death_routine(void *arg)
{
	t_params	*params;

	params = (t_params *)arg;
	printf("The reaper is here...\n");
	//maybe add a delay to synchronise threads
	while (1)
	{
		if (need_stop(params))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}