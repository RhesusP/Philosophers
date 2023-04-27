/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 22:22:35 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/27 13:15:08 by cbernot          ###   ########.fr       */
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
		if (pthread_mutex_init(&(param->forks[i]), NULL) != 0)
		{
			free(param->forks);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_param_mutexes(t_params *param)
{
	if (pthread_mutex_init(&param->is_dead_lock, NULL))
		return (0);
	if (pthread_mutex_init(&param->write_lock, NULL))
		return (0);
	if (pthread_mutex_init(&param->all_eat_lock, NULL))
		return (0);
	return (1);
}

int	check_param_values(t_params *param, int argc)
{
	if (param->nb_philos <= 0 || param->time_to_die <= 0 || \
	param->time_to_eat <= 0 || param->time_to_sleep <= 0 || \
	(argc == 6 && param->max_meal <= 0))
	{
		printf("[!] Error: Arguments cannot be null or negative");
		return (0);
	}
	return (1);
}

int	create_philos(t_params *param)
{
	int	i;

	param->philo_tab = malloc(sizeof(t_philo) * param->nb_philos);
	if (!param->philo_tab)
		return (0);
	i = 0;
	while (i < param->nb_philos)
	{
		if (!init_philo(&(param->philo_tab[i]), i, param))
		{
			free(param->philo_tab);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_params(t_params *param, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("[!] USAGE: ./philo nb_philos die eat sleep [nb_meal]\n");
		return (0);
	}
	inputs_to_params(param, argc, argv);
	if (!check_param_values(param, argc))
		return (0);
	if (!init_param_mutexes(param))
		return (0);
	if (!initialize_forks(param, param->nb_philos))
	{
		if (param->forks)
			free(param->forks);
		return (0);
	}
	if (!create_philos(param))
	{
		free(param->forks);
		return (0);
	}	
	return (1);
}
