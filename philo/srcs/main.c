/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/04 14:23:15 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	wait_threads(t_params param)
{
	int	i;

	i = 0;
	while (i < param.nb_philos)
	{
		pthread_join(param.philo_tab[i].thread, NULL);
		i++;
	}
	pthread_join(param.death_checker, NULL);
}

void	launch_threads(t_params *params)
{
	int	i;

	params->start_ts = get_current_ts();
	i = 0;
	while (i < params->nb_philos)
	{
		params->philo_tab[i].last_meal_ts = params->start_ts;
		pthread_create(&(params->philo_tab[i].thread), NULL,
			philo_routine, &(params->philo_tab[i]));
		i++;
	}
	pthread_create(&(*params).death_checker, NULL,
		death_checker_routine, params);
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (!init_params(&params, argc, argv))		// TODO catch miss-malloc
		return (0);
	launch_threads(&params);
	wait_threads(params);
	free_stuff(&params);
	return (0);
}
