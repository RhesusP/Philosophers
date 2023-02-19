/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:41:47 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/19 23:37:32 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo_bonus.h"

int	init_params(int argc, char **argv, t_params *params)
{
	printf("\033[31minit params called\n\033[39m");
	if (argc < 5 || argc > 6)
	{
		printf("[!] USAGE: ./philo nb_philos die eat sleep [nb_meal]\n");
		return (0);
	}
	params->nb_philos = ft_atoi(argv[1]);
	params->time_to_die = ft_atoi(argv[2]);
	params->time_to_eat = ft_atoi(argv[3]);
	params->time_to_sleep = ft_atoi(argv[4]);
	params->max_meal = -1;
	if (argc == 6)
		params->max_meal = ft_atoi(argv[5]);
	params->start_ts = 0;
	params->pids = malloc(sizeof(pid_t) * params->nb_philos);
	if (!params->pids)
		return (0);
	sem_init(&params->forks, 0, params->nb_philos);
	return (1);
}

t_philo	*init_philo(t_params *params, int id)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (0);
	philo->id = id;
	philo->last_meal_ts = 0;
	philo->nb_meal = 0;
	philo->params = params;
	return (philo);
}

void	launch_forks(t_params *params)
{
	int		i;
	pid_t	pid;
	t_philo	*philo;

	i = 0;
	params->start_ts = get_current_ts();
	while (i < params->nb_philos)
	{
		pid = fork();
		if (pid == 0)	// here is the son
		{
			philo = init_philo(params, i + 1);
			philo_routine(philo);
			exit(EXIT_SUCCESS);
		}
		if (pid > 0)
		{
			params->pids[i] = pid;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (!init_params(argc, argv, &params))
		return (0);
	launch_forks(&params);
	wait(NULL);
	int	i = 0;
	while (i < params.nb_philos)
	{
		printf("pid[%d]: %d\n", i, params.pids[i]);
		i++;
	}
	printf("Father finished\n");
	return (0);
}