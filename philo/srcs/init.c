/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 19:04:54 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/12 17:16:39 by cbernot          ###   ########.fr       */
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
	if (pthread_mutex_init(&param->is_dead_lock, NULL))
		return (0);
	pthread_mutex_unlock(&param->is_dead_lock);
	if (pthread_mutex_init(&param->write_lock, NULL))
		return (0);
	pthread_mutex_unlock(&param->write_lock);
	forks_ret = initialize_forks(param, param->nb_philos);
	if (param->nb_philos <= 0 || param->time_to_die <= 0 || param->time_to_eat <= 0 || param->time_to_sleep <= 0 || (argc == 6 && param->max_meal <= 0) || forks_ret == 0)
	{
		if (param->forks)
			free(param->forks);
		return (0);
	}
	return (1);
}

/**
 * @brief Initialize fields of a single philosophers.
 * 
 * @param philo Pointer to a t_philo. 
 * @param id Philosopher identifier. 
 * @param param Pointer to t_params.
 * @return 1 if mutex properly initialized. 0 if not.
 */
int	init_philo(t_philo *philo, int id, t_params *param)
{
	philo->id = id + 1;
	philo->last_meal_ts = 0;
	if (pthread_mutex_init(&philo->last_meal_lock, NULL))
		return (0);
	philo->nb_meal = 0;
	philo->params = param;
	philo->left_fork = &param->forks[id];
	if (id - 1 < 0)
		philo->right_fork = &param->forks[param->nb_philos - 1];
	else
		philo->right_fork = &param->forks[id - 1];
	return (1);
}

/**
 * @brief Initialize a t_philo array thanks to param rules.
 * 
 * @param philos Pointer to t_philo array.
 * @param param Pointer to t_params
 * @return 1 of evrything is properly allocated. 0 if not.
 */
int	create_philos_array(t_philo **philos, t_params *param)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * param->nb_philos);
	if (!*philos)
		return (0);
	i = 0;
	while (i < param->nb_philos)
	{
		if (!init_philo(&(*philos)[i], i, param))
		{
			free(param->forks);
			free(*philos);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Launch threads reprensenting philosophers and one death checker.
 * 
 * @param params Pointer to t_params.
 * @param philos Pointer to t_philo array.
 */
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
