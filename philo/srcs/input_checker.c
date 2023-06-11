/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 16:41:15 by cbernot           #+#    #+#             */
/*   Updated: 2023/06/08 17:21:39 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

static int	ft_get_neg_int_len(int nbr)
{
	int	len_nbr;

	len_nbr = 0;
	while (nbr < 0)
	{
		nbr /= 10;
		len_nbr++;
	}
	return (len_nbr + 1);
}

static int	is_valid_int(char *str)
{
	int	len_str;
	int	len_nbr;
	int	nbr;

	len_str = ft_strlen(str);
	nbr = ft_atoi(str);
	len_nbr = 0;
	if (nbr <= 0)
		len_nbr = ft_get_neg_int_len(nbr);
	else
	{
		while (nbr > 0)
		{
			nbr /= 10;
			len_nbr++;
		}
	}
	if (len_nbr != len_str)
		return (0);
	return (1);
}

int	is_input_valid(t_params *param, char **argv, int argc)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_int(argv[i]))
		{
			printf("Error: '%s' is not a valid argument.\n", argv[i]);
			return (0);
		}
		i++;
	}
	param->nb_philos = ft_atoi(argv[1]);
	param->time_to_die = ft_atoi(argv[2]);
	param->time_to_eat = ft_atoi(argv[3]);
	param->time_to_sleep = ft_atoi(argv[4]);
	param->max_meal = -1;
	if (argc == 6)
		param->max_meal = ft_atoi(argv[5]);
	if (!(param->nb_philos >= 0 && param->time_to_die >= 0 && \
		param->time_to_eat >= 0 && param->time_to_sleep >= 0))
		return (print_error("parameters must be a positive integer."));
	if (argc == 6 && !(param->max_meal >= 0))
		return (print_error("parameters must be a positive integer."));
	if (param->nb_philos == 0)
		return (print_error("simulation must have at least one philosopher."));
	if (param->max_meal == 0)
		return (0);
	return (1);
}
