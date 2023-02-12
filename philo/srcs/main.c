/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/12 02:31:18 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	wait_threads(t_philo **philos, int nb_philos)
{
	int	i;

	i = 0;
	while (i < nb_philos)
	{
		pthread_join((*philos)[i].thread, NULL);
		i++;
	}
	pthread_join((*philos)[0].params->death_checker, NULL);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		**philos;

	if (!init_params(&params, argc, argv))
		return (0);
	philos = malloc(sizeof(t_philo *));
	if (!philos)
		return (0);
	if (!create_philos_array(philos, &params))
	{
		free(philos);
		return (0);
	}
	launch_threads(&params, philos);
	wait_threads(philos, params.nb_philos);
	free_stuff(philos, &params);
	return (0);
}
