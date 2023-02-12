/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:03:05 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/12 01:08:39 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

static int	ft_isspace(const char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		res;
	size_t	i;
	int		sign;

	res = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + str[i] - 48;
		i++;
	}
	return (res * sign);
}

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
	pthread_mutex_lock(&philo->params->write_lock);
	if (!is_philo_dead(philo))
		printf("%lld %d %s\n", current_time, philo->id, action);
	pthread_mutex_unlock(&philo->params->write_lock);
}

int	is_philo_dead(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(&philo->params->is_dead_lock);
	is_dead = philo->params->is_dead;
	pthread_mutex_unlock(&philo->params->is_dead_lock);
	return (is_dead);
}
