/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 01:18:32 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/27 13:05:45 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

void	free_stuff(t_params *param)
{
	free(param->forks);
	free(param->philo_tab);
}

void	inputs_to_params(t_params *param, int argc, char **argv)
{
	param->nb_philos = ft_atoi(argv[1]);
	param->time_to_die = ft_atoi(argv[2]);
	param->time_to_eat = ft_atoi(argv[3]);
	param->time_to_sleep = ft_atoi(argv[4]);
	param->all_ate = 0;
	param->max_meal = -1;
	if (argc == 6)
		param->max_meal = ft_atoi(argv[5]);
	param->start_ts = 0;
	param->is_dead = 0;
}
