/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 01:18:32 by cbernot           #+#    #+#             */
/*   Updated: 2023/04/04 14:08:19 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Free forks and philos.
 * 
 * @param philos 
 * @param param 
 */
void	free_stuff(t_params *param)
{
	free(param->forks);
	free(param->philo_tab);
}
