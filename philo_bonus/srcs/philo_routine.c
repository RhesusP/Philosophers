/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:09:55 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/22 17:58:33 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo_bonus.h"

void	take_forks(t_philo *philo)
{
	//sem_t	sem;

	int	forks_left;

	sem_getvalue(philo->params->forks, &forks_left);
	printf("[init]: there is %d forks left\n", forks_left);
	sem_wait(philo->params->forks);
	print_action(philo, "has taken a fork");
	sem_getvalue(philo->params->forks, &forks_left);
	printf("there is %d forks left\n", forks_left);
	sem_wait(philo->params->forks);
	print_action(philo, "has taken a fork");
	sem_getvalue(philo->params->forks, &forks_left);
	printf("there is %d forks left\n", forks_left);
}

void	ft_sleep(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->params->time_to_sleep);	
}

void	ft_think(t_philo *philo)
{
	print_action(philo, "is thinking");
}

void	philo_routine(t_philo *philo)
{
	int		max_meal;

	max_meal = philo->params->max_meal;
	while (!is_philo_dead(philo))
	{
		if (max_meal >= 0 && philo->nb_meal >= max_meal)
			break ;
		take_forks(philo);
		philo->last_meal_ts = get_current_ts();
		print_action(philo, "\033[95mis eating\033[39m");
		ft_usleep(philo->params->time_to_eat);
		sem_post(philo->params->forks);
		sem_post(philo->params->forks);
		philo->nb_meal++;
		ft_sleep(philo);
		ft_think(philo);
	}
	return ;
}