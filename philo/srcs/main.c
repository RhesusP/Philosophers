/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/05/26 08:36:04 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

static void	wait_threads(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->nb_philos)
	{
		pthread_join(params->philo_tab[i].thread, NULL);
		i++;
	}
	if (params->nb_philos > 1)
		pthread_join(params->death_checker, NULL);
}

static int	launch_threads(t_params *params)
{
	int	i;

	params->start_ts = get_current_ts();
	i = 0;
	while (i < params->nb_philos)
	{
		if (pthread_create(&params->philo_tab[i].thread, NULL, \
			&philo_routine, &params->philo_tab[i]) != 0)
			return (print_error("one of the philosopher threads \
				failed to launch."));
		i++;
	}
	if (params->nb_philos > 1)
	{
		if (pthread_create(&params->death_checker, NULL, \
			&death_routine, params) != 0)
			return (print_error("death checker thread failed to launch."));
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (!init_params(&params, argc, argv))
		return (EXIT_FAILURE);
	if (!launch_threads(&params))
		return (EXIT_FAILURE);
	wait_threads(&params);
	free(params.philo_tab);
	free(params.forks);
	return (EXIT_SUCCESS);
}
