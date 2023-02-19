/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:24:25 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/19 23:27:24 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo_bonus.h"

unsigned long long	get_current_ts(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

void	ft_usleep(unsigned long long ms_duration)
{
	unsigned long long	start_time;

	start_time = get_current_ts();
	while ((get_current_ts() - start_time) < ms_duration)
		usleep(100);
}

void	print_action(t_philo *philo, char *action)
{
	unsigned long long	current_time;

	current_time = get_current_ts() - philo->params->start_ts;
	//pthread_mutex_lock(&philo->params->write_lock);
	//if (!is_philo_dead(philo))
	printf("%lld %d %s\n", current_time, philo->id, action);
	//pthread_mutex_unlock(&philo->params->write_lock);
}