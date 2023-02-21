/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 22:22:35 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/21 22:38:19 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Initialize nb_philos mutexes representing the forks.
 * 
 * @param param A pointer to t_params 
 * @param nb_philos Number of philosophers (= number of forks).
 * @return 1 if evrything is properly allocated. 0 if not.
 */
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

/**
 * @brief Initialize param mutexes.
 * 
 * @param param 
 * @return 1 if mutexes are properly created. 0 if not. 
 */
int	init_param_mutexes(t_params *param)
{
	if (pthread_mutex_init(&param->is_dead_lock, NULL))
		return (0);
	pthread_mutex_unlock(&param->is_dead_lock);
	if (pthread_mutex_init(&param->write_lock, NULL))
		return (0);
	pthread_mutex_unlock(&param->write_lock);
	return (1);
}

/**
 * @brief Check if all param values are positive and non null integer
 * 
 * @param param 
 * @param argc 
 * @param forks_ret 
 * @return 1 if all tests have been passed. 0 if at least one failed.
 */
int	check_param_values(t_params *param, int argc, int forks_ret)
{
	if (param->nb_philos <= 0 || param->time_to_die <= 0)
		return (0);
	if (param->time_to_eat <= 0 || param->time_to_sleep <= 0)
		return (0);
	if ((argc == 6 && param->max_meal <= 0) || forks_ret == 0)
	{
		if (param->forks)
			free(param->forks);
		return (0);
	}
	return (1);
}

/**
 * @brief Initialize param struct thanks to argc and argv.
 * 
 * @param param Pointer to a t_params
 * @param argc
 * @param argv 
 * @return 1 if evrything is properly allocated. 0 if not.
 */
int	init_params(t_params *param, int argc, char **argv)
{
	int	forks_ret;

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
	if (!init_param_mutexes(param))
		return (0);
	forks_ret = initialize_forks(param, param->nb_philos);
	if (!check_param_values(param, argc, forks_ret))
		return (0);
	return (1);
}
