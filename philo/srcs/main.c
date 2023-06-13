/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/11 22:20:01 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

int	launch_threads(t_params *param)
{
	int	i;

	i = 0;
	param->start_time = get_current_ts() + (param->nb_philos * 10 * 2);
	while (i < param->nb_philos)
	{
		param->philos[i].last_meal_ts = param->start_time;
		if (pthread_create(&param->philos[i].thread, NULL, &philo_routine, &param->philos[i]) != 0)
			return (print_error("thread creation failed."));
		i++;
	}
	if (param->nb_philos > 1)
	{
		if (pthread_create(&param->death_checker, NULL, &death_routine, param) != 0)
			return (print_error("thread creation failed."));
	}
	return (1);
}

int	wait_threads(t_params *param)
{
	int	i;

	i = 0;
	while (i < param->nb_philos)
	{
		if (pthread_join(param->philos[i].thread, NULL) != 0)
			return (print_error("something gone wrong while waiting for threads."));
		i++;
	}
	if (param->nb_philos > 1)
	{
		if (pthread_join(param->death_checker, NULL) != 0)
			return (print_error("something gone wrong while waiting for death checker threads."));
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	*param;

	param = init_params(argc, argv);
	if (!param)
		return (EXIT_FAILURE);
	if (!launch_threads(param))
		return (EXIT_FAILURE);
	if (!wait_threads(param))
		return (EXIT_FAILURE);
	free(param);
	return (EXIT_SUCCESS);
}

	//destroy mutex on error and at end ?