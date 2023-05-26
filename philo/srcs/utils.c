/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:57:46 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/26 08:43:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

unsigned long long	get_current_ts(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

void	ft_sleep(t_params *params, unsigned long long time)
{
	unsigned long long	limit;

	limit = get_current_ts() + time;
	while (get_current_ts() < limit)
	{
		if (is_stopped(params))
			break ;
		usleep(100);
	}
}

void	print_action(t_philo *philo, char *action)
{
	unsigned long long	time;

	pthread_mutex_lock(&philo->params->write_lock);
	if (is_stopped(philo->params))
	{
		pthread_mutex_unlock(&philo->params->write_lock);
		return ;
	}
	time = get_current_ts() - philo->params->start_ts;
	printf("%lld %d %s\n", time, philo->id, action);
	pthread_mutex_unlock(&philo->params->write_lock);
}

int	is_philo_dead(t_philo *philo)
{
	unsigned long long	time_to_die;

	time_to_die = (unsigned long long)philo->params->time_to_die;
	if (get_current_ts() - philo->last_meal_ts >= time_to_die)
	{
		pthread_mutex_lock(&philo->params->stop_lock);
		philo->params->stop = 1;
		pthread_mutex_unlock(&philo->params->stop_lock);
		return (1);
	}
	return (0);
}
