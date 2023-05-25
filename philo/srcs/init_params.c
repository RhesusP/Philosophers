/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 22:22:35 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/25 16:46:42 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	print_error(char *msg)
{
	printf("Error: %s\n", msg);
	return (0);
}

static int	init_mutexes(t_params *params)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&params->stop_lock, NULL) != 0)
		return (print_error("mutex failed to initialize."));
	if (pthread_mutex_init(&params->write_lock, NULL) != 0)
		return (print_error("mutex failed to initialize."));
	params->forks = malloc(sizeof(pthread_mutex_t) * (params->nb_philos + 1));
	if (!params->forks)
		return (print_error("malloc failed."));
	while (i < params->nb_philos)
	{
		if (pthread_mutex_init(&params->forks[i], NULL) != 0)
		{
			free(params->forks);
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

static int	init_philos(t_params *params)
{
	int	i;

	params->philo_tab = malloc(sizeof(t_philo) * (params->nb_philos + 1));
	if (!params->philo_tab)
	{
		free(params->forks);
		return (print_error("malloc failed."));
	}
	i = 0;
	while (i < params->nb_philos)
	{
		params->philo_tab[i].id = i + 1;
		params->philo_tab[i].meals_nb = 0;
		params->philo_tab[i].params = params;
		if (pthread_mutex_init(&params->philo_tab[i].last_meal_lock, NULL) != 0)
		{
			free(params->forks);
			free(params->philo_tab);
			return (print_error("mutex failed to initialize."));
		}
		get_forks(&params->philo_tab[i], params->nb_philos);
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
	if (!is_input_valid(param, argv, argc))
		return (0);
	param->stop = 0;
	if (!init_mutexes(param))
		return (0);
	if (!init_philos(param))
		return (0);
	return (1);
}
