/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:04:54 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/12 02:15:13 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	initialize_forks(t_params *param, int nb_philos)
{
	int	i;

	i = 0;
	param->forks = malloc(sizeof(pthread_mutex_t) * nb_philos);
	if (!param->forks)
		return (0);
	while (i < nb_philos)
	{
		pthread_mutex_init(&(param->forks[i]), NULL);
		i++;
	}
	return (1);
}

/**
 * @brief 
 * 
 * @param param 
 * @param argc 
 * @param argv 
 * @return 0 if error, 1 if all is good. 
 */
int	init_params(t_params *param, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("[!] USAGE: ./philo nb_philos die eat sleep [nb_meal]\n");
		return (0);
	}
	param->nb_philos = ft_atoi(argv[1]);
	param->time_to_die = ft_atoi(argv[2]);
	param->time_to_eat = ft_atoi(argv[3]);
	param->time_to_sleep = ft_atoi(argv[4]);
	param->max_meal = -1;
	if (argc == 6)
		param->max_meal = ft_atoi(argv[5]);
	param->start_ts = 0;
	param->is_dead = 0;
	pthread_mutex_init(&param->is_dead_lock, NULL);
	pthread_mutex_unlock(&param->is_dead_lock);
	pthread_mutex_init(&param->write_lock, NULL);
	pthread_mutex_unlock(&param->write_lock);
	if (param->nb_philos <= 0 || param->time_to_die <= 0 || param->time_to_eat <= 0 || param->time_to_sleep <= 0 || (argc == 6 && param->max_meal <= 0) || initialize_forks(param, param->nb_philos) == 0)
		return (0);
	return (1);
}

void	init_philo(t_philo *philo, int id, t_params *param)
{

	philo->id = id + 1;
	philo->last_meal_ts = 0;
	pthread_mutex_init(&philo->last_meal_lock, NULL);
	philo->nb_meal = 0;
	philo->params = param;
	philo->left_fork = &param->forks[id];
	if (id - 1 < 0)
		philo->right_fork = &param->forks[param->nb_philos - 1];
	else
		philo->right_fork = &param->forks[id - 1];
}

int	create_philos_array(t_philo **philos, t_params *param)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * param->nb_philos);
	if (!*philos)
		return (0);
	i = 0;
	while (i < param->nb_philos)
	{
		init_philo(&(*philos)[i], i, param);
		i++;
	}
	return (1);
}

void	launch_threads(t_params *params, t_philo **philos)
{
	int	i;

	params->start_ts = get_current_ts();
	i = 0;
	while (i < params->nb_philos)
	{
		(*philos)[i].last_meal_ts = params->start_ts;
		pthread_create(&((*philos)[i].thread), NULL, philo_routine, &((*philos)[i]));
		i++;
	}
	pthread_create(&(*params).death_checker, NULL, death_checker_routine, philos);
}