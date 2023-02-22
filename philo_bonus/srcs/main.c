/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:41:47 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/22 18:47:12 by cbernot          ###   ########.fr       */
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
	//params->forks = malloc(sizeof(sem_t));
	params->write_lock = sem_open("write_lock", O_CREAT, 0644, 1);
	params->is_dead_lock = sem_open("dead_lock", O_CREAT, 0644, 1);
	params->is_dead = 0;
	printf("nb philos: %d\n", params->nb_philos);
	params->forks = sem_open("sem_fork", O_CREAT, 0644, params->nb_philos + 10);
	int	val;
	sem_getvalue(params->forks, &val);
	printf("semaphore value: %d\n", val);
	return (1);
}



void	launch_death_checker(t_params *params, t_philo **philos)
{
	pthread_create(&(*params).death_checker, NULL,
		death_checker_routine, philos);
}

void	launch_forks(t_params *params, t_philo **philos)
{
	int		i;
	pid_t	pid;

	i = 0;
	params->start_ts = get_current_ts();
	while (i < params->nb_philos)
	{
		pid = fork();
		if (pid == 0)	// here is the son
		{
			philo_routine(&((*philos)[i]));
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
	t_philo		**philos;

	if (!init_params(argc, argv, &params))
		return (0);
	philos = malloc(sizeof(t_philo *));
	if (!philos)
		return (0);
	if (!create_philos_array(philos, &params))
	{
		free(philos);
		free(params.forks);
		return (0);
	}
	launch_forks(&params, philos);
	launch_death_checker(&params, philos);
	int	i = 0;
	while (i < params.nb_philos)
	{
		waitpid(params.pids[i], NULL, 0);
		i++;
	}
	pthread_join(params.death_checker, NULL);
	sem_unlink("write_lock");
	sem_close(params.write_lock);

	sem_unlink("dead_lock");
	sem_close(params.is_dead_lock);

	sem_unlink("sem_fork");
	sem_close(params.forks);
	printf("Father finished\n");
	return (0);
}