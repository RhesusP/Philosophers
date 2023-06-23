/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:57:46 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/23 12:54:28 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Get the current timestamp in milliseconds.
 * 
 * @return unsigned long long 
 */
unsigned long long	get_current_ts(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

/**
 * @brief Check if the simulation is stopped.
 * 
 * @param param 
 * @return 1 if stopped, 0 if not.
 */
int	is_stopped(t_params *param)
{
	int	stop;

	pthread_mutex_lock(&param->is_dead_lock);
	stop = param->is_dead;
	pthread_mutex_unlock(&param->is_dead_lock);
	return (stop);
}

/**
 * @brief Sleep for a given time.
 * C usleep is not precise, so we use a loop to to sleep for 100 microseconds 
 * and check if the simulation is stopped during the sleep.
 * @param params 
 * @param time 
 */
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

/**
 * @brief Print the action of a philosopher.
 * Lock the mutex to avoid multiple philosophers writing at the same time and
 * do not print if the simulation is stopped.
 * @param philo current philosopher
 * @param status the current philosopher's action
 */
void	print_action(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->param->write_lock);
	if (is_stopped(philo->param))
	{
		pthread_mutex_unlock(&philo->param->write_lock);
		return ;
	}
	printf("%lld %d %s\n", get_current_ts() - philo->param->start_time, \
	philo->id, status);
	pthread_mutex_unlock(&philo->param->write_lock);
}
