/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:09:55 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/19 23:33:20 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo_bonus.h"

void	take_forks(t_philo *philo)
{
	sem_t	sem;

	int	forks_left;

	printf("[%d] nb philo: %d\n", philo->id, philo->params->nb_philos);
	sem_getvalue(&sem, &forks_left);
	printf("[init]: there is %d forks left\n", forks_left);
	sem_wait(&sem);
	print_action(philo, "has taken a fork");
	sem_getvalue(&sem, &forks_left);
	printf("there is %d forks left\n", forks_left);
	sem_wait(&sem);
	print_action(philo, "has taken a fork");
	sem_getvalue(&sem, &forks_left);
	printf("there is %d forks left\n", forks_left);
}

/*
void	ft_sleep(t_philo *philo)
{
	
}

void	ft_think(t_philo *philo)
{
	
}
*/

void	philo_routine(t_philo *philo)
{
	//int	max_meal;

	printf("I'm philo %d\n", philo->id);
	//max_meal = philo->params->max_meal;
	take_forks(philo);
	return ;
}