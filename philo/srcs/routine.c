/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:21:23 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/05 19:58:27 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

unsigned long long	get_ms_time(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

void	ft_usleep(t_philo *philo, unsigned long long time)
{
	int	i;
	
	i = 0;
	while (i < 1000)
	{
		if (get_ms_time() - philo->last_meal > philo->time_to_die)
		{
			pthread_mutex_lock(philo->life_lock);
			philo->is_alive = 0;
			pthread_mutex_unlock(philo->life_lock);
			break;
		}
		usleep(time);
		i++;
	}
}

void	ft_take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->first_lock);
	if (!*philo->is_first)
	{
		pthread_mutex_unlock(philo->first_lock);
		return ;
	}
	pthread_mutex_unlock(philo->first_lock);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left);
		printf("[%lld ms] %d has taken a fork\n", get_ms_time() - philo->birth, philo->id);
		pthread_mutex_lock(philo->right);
		printf("[%lld ms] %d has taken a fork\n", get_ms_time() - philo->birth, philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->right);
		printf("[%lld ms] %d has taken a fork\n", get_ms_time() - philo->birth, philo->id);
		pthread_mutex_lock(philo->left);
		printf("[%lld ms] %d has taken a fork\n", get_ms_time() - philo->birth, philo->id);
	}
}

unsigned long long	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->first_lock);
	if (!*philo->is_first)
	{
		pthread_mutex_unlock(philo->first_lock);
		return (0);
	}
	pthread_mutex_unlock(philo->first_lock);

	philo->last_meal = get_ms_time();
	printf("[%lld ms] %d is eating\n", philo->last_meal - philo->birth, philo->id);
	ft_usleep(philo, philo->time_to_eat);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	return (philo->last_meal);
}

void	ft_sleep(t_philo *philo)
{
	pthread_mutex_lock(philo->first_lock);
	if (!*philo->is_first)
	{
		pthread_mutex_unlock(philo->first_lock);
		return ;
	}
	pthread_mutex_unlock(philo->first_lock);

	printf("[%lld ms] %d is sleeping\n", get_ms_time() - philo->birth, philo->id);
	ft_usleep(philo, philo->time_to_sleep);
}

void	ft_think(t_philo *philo)
{
	pthread_mutex_lock(philo->first_lock);
	if (!*philo->is_first)
	{
		pthread_mutex_unlock(philo->first_lock);
		return ;
	}
	pthread_mutex_unlock(philo->first_lock);
	printf("[%lld ms] %d is thinking\n", get_ms_time() - philo->birth, philo->id);
}

void	*thread_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->birth = get_ms_time();

	while (philo->is_alive && *philo->is_first)
	{
		ft_take_forks(philo);
		philo->last_meal = ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	pthread_mutex_lock(philo->first_lock);
	if (*philo->is_first)
	{
		printf("\033[0;31m[%lld ms] %d is dead\n\033[0m",get_ms_time() - philo->birth, philo->id);
		*philo->is_first = 0;
	}
	pthread_mutex_unlock(philo->first_lock);
	return (0);
}