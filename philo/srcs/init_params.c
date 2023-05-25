/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 22:22:35 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/24 20:26:12 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	print_error(char *msg)
{
	printf("Error: %s\n", msg);
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

int	ft_get_neg_int_len(int nbr)
{
	int	len_nbr;

	len_nbr = 0;
	while (nbr < 0)
	{
		nbr /= 10;
		len_nbr++;
	}
	return (len_nbr + 1);
}

int	is_valid_int(char *str)
{
	int	len_str;
	int	len_nbr;
	int	nbr;

	len_str = ft_strlen(str);
	nbr = ft_atoi(str);
	len_nbr = 0;
	if (nbr <= 0)
		len_nbr = ft_get_neg_int_len(nbr);
	else
	{
		while (nbr > 0)
		{
			nbr /= 10;
			len_nbr++;
		}
	}
	if (len_nbr != len_str)
		return (0);
	return (1);
}

int	is_input_valid(t_params *param, char **argv, int argc)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_int(argv[i]))
		{
			printf("Error: '%s' is not a valid argument.\n", argv[i]);
			return (0);
		}
		i++;
	}
	param->nb_philos = ft_atoi(argv[1]);
	param->time_to_die = ft_atoi(argv[2]);
	param->time_to_eat = ft_atoi(argv[3]);
	param->time_to_sleep = ft_atoi(argv[4]);
	param->max_meal = -1;
	if (argc == 6)
		param->max_meal = ft_atoi(argv[5]);
	if (!(param->nb_philos >= 1 && param->time_to_die >= 1 && param->time_to_eat >= 1 && param->time_to_sleep >= 1))
		return (print_error("parameters must be a positive non-zero integer."));
	if (argc == 6 && !(param->max_meal >= 1))
		return (print_error("parameters must be a positive non-zero integer."));
	return (1);
}

int	init_mutexes(t_params *params)
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

void	get_forks(t_philo *philo, int nb_philo)
{
	philo->right_fork = philo->id - 1;
	if (philo->id - 2 < 0)
		philo->left_fork = nb_philo - 1;
	else
		philo->left_fork = philo->id - 2;
}

int	init_philos(t_params *params)
{
	int	i;

	params->philo_tab = malloc(sizeof(pthread_t) * (params->nb_philos + 1));
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
