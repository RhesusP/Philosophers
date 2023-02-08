/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/05 20:01:12 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

t_params	init_params(t_params param, char **argv)
{
	int	i;

	param.nb_philos = ft_atoi(argv[1]);
	param.died = 0;
	param.time_to_die = ft_atoi(argv[2]);
	param.time_to_eat = ft_atoi(argv[3]);
	param.time_to_sleep = ft_atoi(argv[4]);
	param.is_first = 1;
	param.first_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(param.first_lock, NULL);
	param.threads = malloc(sizeof(pthread_t) * param.nb_philos);
	param.forks = malloc(sizeof(pthread_mutex_t) * param.nb_philos);
	param.philos = malloc(sizeof(t_philo *) * param.nb_philos);
	i = 0;
	while (i < param.nb_philos)
	{
		pthread_mutex_init(&param.forks[i], NULL);
		i++;
	}
	return (param);
}

t_philo	*create_philo(int id, t_params *param)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (0);
	philo->id = id + 1;
	philo->is_alive = 1;
	philo->life_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->life_lock, NULL);
	philo->first_lock = malloc(sizeof(pthread_mutex_t));
	philo->first_lock = param->first_lock;
	philo->is_first = &param->is_first;
	philo->left = &param->forks[id];
	if (id - 1 < 0)
		philo->right = &param->forks[param->nb_philos - 1];
	else
		philo->right = &param->forks[id - 1];
	philo->time_to_die = param->time_to_die;
	philo->time_to_eat = param->time_to_eat;
	philo->time_to_sleep = param->time_to_sleep;
	return (philo);
}

void	*monitor_routine(void *p)
{
	t_params	*param;
	int			i;
	int			j;

	param = (t_params*)p;
	//printf("\033[0;34mhey, I'm the monitor\n\033[0m");
	while (!param->died)
	{
		//printf("\033[0;34mMonitor loop\n\033[0m");
		i = 0;
		while (i < param->nb_philos && !param->died)
		{
			if (!param->philos[i]->is_alive)
			{
				printf("\033[0;34m%d PHILO EST MORT\n\033[0m", i + 1);
				j = 0;
				while (j < param->nb_philos)
				{
					param->died = 1;
					pthread_mutex_lock(param->philos[j]->life_lock);
					param->philos[j]->is_alive = 0;
					pthread_mutex_unlock(param->philos[j]->life_lock);
					j++;
				}
				return (0);
			}
			i++;
		}
	}
	//printf("\033[0;34mMonitor end\n\033[0m");
	return (0);
}

void	launch_threads(t_params	*param)
{
	t_philo		*philo;
	pthread_t	monitor;
	int			i;

	i = 0;
	while (i < param->nb_philos)
	{
		philo = create_philo(i, param);
		param->philos[i] = philo;
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, param);
	pthread_detach(monitor);
	i = 0;
	while (i < param->nb_philos)
	{
		pthread_create(&param->threads[i], NULL, thread_routine, param->philos[i]);
		i++;
	}
	i = 0;
	while (i < param->nb_philos)
	{
		pthread_join(param->threads[i], NULL);
		i++;
	}
}


int	main(int argc, char **argv)
{
	t_params	params;

	if (argc < 4 || argc > 5)
	{
		printf("usage error\n");
		return (0);
	}
	params = init_params(params, argv);
	launch_threads(&params);
	return (0);
}