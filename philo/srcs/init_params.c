/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 22:22:35 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/14 11:29:12 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	print_error(char *msg)
{
	printf("Error: %s\n", msg);
	return (0);
}

static int	init_mutexes(t_params *param)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&param->write_lock, NULL) != 0)
		return (print_error("mutex failed to initialize."));
	if (pthread_mutex_init(&param->is_dead_lock, NULL) != 0)
		return (print_error("mutex failed to initialize."));
	param->forks = malloc(sizeof(pthread_mutex_t) * (param->nb_philos + 1));
	if (!param->forks)
	{
		free(param->philos);
		return (print_error("malloc failed."));
	}
	while (i < param->nb_philos)
	{
		if (pthread_mutex_init(&param->forks[i], NULL) != 0)
		{
			free(param->philos);
			free(param->forks);
			return (print_error("mutex failed to initialize."));
		}
		i++;
	}
	return (1);
}

static void	get_forks(t_philo *philo, int nb_philo)
{
	philo->right_fork = philo->id - 1;
	if (philo->id - 2 < 0)
		philo->left_fork = nb_philo - 1;
	else
		philo->left_fork = philo->id - 2;
}

static int	init_philos(t_params *param)
{
	int	i;

	param->philos = malloc(sizeof(t_philo) * (param->nb_philos));
	if (!param->philos)
		return (print_error("malloc failed."));
	i = 0;
	while (i < param->nb_philos)
	{
		param->philos[i].id = i + 1;
		param->philos[i].nb_meals = 0;
		param->philos[i].param = param;
		get_forks(&param->philos[i], param->nb_philos);
		if (pthread_mutex_init(&param->philos[i].last_meal_lock, NULL) != 0)
		{
			free(param->philos);
			return (0);
		}
		i++;
	}
	return (1);
}

t_params	*init_params(int argc, char **argv)
{
	t_params	*param;

	param = malloc(sizeof(t_params));
	if (!param)
		return (0);
	if (argc < 5 || argc > 6)
	{
		printf("[!] USAGE: ./philo nb_philos die eat sleep [nb_meal]\n");
		return (0);
	}
	if (!is_input_valid(param, argv, argc) || !init_philos(param))
	{
		free(param);
		return (0);
	}	
	param->is_dead = 0;
	if (!init_mutexes(param))
	{
		free(param);
		return (0);
	}
	return (param);
}
