/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:57:46 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/21 22:27:56 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Get the current in millisecondes.
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
 * @brief Smart usleep.
 * 
 * @param ms_duration Duration in millisecondes.
 */
void	ft_usleep(unsigned long long ms_duration)
{
	unsigned long long	start_time;

	start_time = get_current_ts();
	while ((get_current_ts() - start_time) < ms_duration)
		usleep(100);
}

/**
 * @brief Securely prints philospher's action in the following format:
 * "ms_timestamp id action"
 * 
 * @param philo Pointer to a philopher.
 * @param action Action to print.
 */
void	print_action(t_philo *philo, char *action)
{
	unsigned long long	current_time;

	current_time = get_current_ts() - philo->params->start_ts;
	pthread_mutex_lock(&philo->params->write_lock);
	if (!is_philo_dead(philo))
		printf("%lld %d %s\n", current_time, philo->id, action);
	pthread_mutex_unlock(&philo->params->write_lock);
}

/**
 * @brief Securely check and return is_dead variable.
 * 
 * @param philo Pointer to a philosopher.
 * @return is_dead value.
 */
int	is_philo_dead(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(&philo->params->is_dead_lock);
	is_dead = philo->params->is_dead;
	pthread_mutex_unlock(&philo->params->is_dead_lock);
	return (is_dead);
}
