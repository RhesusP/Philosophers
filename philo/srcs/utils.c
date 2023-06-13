/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:57:46 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/11 13:10:51 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

unsigned long long	get_current_ts(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

int	is_stopped(t_params *param)
{
	int	stop;

	pthread_mutex_lock(&param->is_dead_lock);
	stop = param->is_dead;
	pthread_mutex_unlock(&param->is_dead_lock);
	return (stop);
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

void	print_action(t_philo *philo, char *status, int force)
{
	pthread_mutex_lock(&philo->param->write_lock);
	if (!force && is_stopped(philo->param))
	{
		pthread_mutex_unlock(&philo->param->write_lock);
		return ;
	}
	printf("%lld %d %s\n", get_current_ts() - philo->param->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->param->write_lock);
}