/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/21 22:26:44 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Wait for each philosopher and for the death_checker.
 * 
 * @param philos 
 * @param nb_philos Number of philosophers.
 */
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
		pthread_create(&((*philos)[i].thread), NULL,
			philo_routine, &((*philos)[i]));
		i++;
	}
	pthread_create(&(*params).death_checker, NULL,
		death_checker_routine, philos);
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
		free(params.forks);
		return (0);
	}
	launch_threads(&params, philos);
	wait_threads(philos, params.nb_philos);
	free_stuff(philos, &params);
	return (0);
}
