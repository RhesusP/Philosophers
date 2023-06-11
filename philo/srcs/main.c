/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 13:50:11 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/11 12:50:57 by cbernot          ###   ########.fr       */
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

void	debug_print(t_params *param)
{
	printf("nb_philos: %d\n", param->nb_philos);
    printf("time_to_die: %d\n", param->time_to_die);
    printf("time_to_eat: %d\n", param->time_to_eat);
    printf("time_to_sleep: %d\n", param->time_to_sleep);
    printf("max_meal: %d\n", param->max_meal);
    printf("start_time: %llu\n", param->start_time);
    printf("is_dead: %d\n", param->is_dead);
	 for (int i = 0; i < param->nb_philos; i++) {
        t_philo *philo = &(param->philos[i]);
        printf("Philosopher %d:\n", i + 1);
        printf("   id: %d\n", philo->id);
        printf("   nb_meals: %d\n", philo->nb_meals);
        printf("   last_meal_ts: %llu\n", philo->last_meal_ts);
        printf("   left_fork: %d\n", philo->left_fork);
        printf("   right_fork: %d\n", philo->right_fork);
    }
}

int	main(int argc, char **argv)
{
	t_params	*param;

	param = init_params(argc, argv);
	if (!param)
		return (EXIT_FAILURE);
	// debug_print(param);
	//destroy mutex on error and at end ?
	if (!launch_threads(param))
		return (EXIT_FAILURE);
	if (!wait_threads(param))
		return (EXIT_FAILURE);
	free(param);
	return (EXIT_SUCCESS);
}
