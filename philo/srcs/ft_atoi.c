/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 11:57:31 by cbernot           #+#    #+#             */
/*   Updated: 2023/02/21 22:35:01 by cbernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/philo.h"

/**
 * @brief Tests for a space character. Character given in parameter 
 * must be in the following set:
 *	 ``0''``1''``2''``3''``4''
     ``5''``6''``7''``8''``9''
 * 
 * @param c 
 * @return Zero if the character tests false and return non-zero if the
 * character tests true.
 */
static int	ft_isspace(const char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

/**
 * @brief Tests for a decimal digit character. Character given in parameter 
 * must be in the following set:
 *	 ``\t''``\n'' `\v''``\f''``\r'' and `` ``
 * 
 * @param c 
 * @return Zero if the character tests false and return non-zero if the
 * character tests true.
 */
int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

/**
 * @brief Converts the initial portion of the string pointed to by str to 
 * int representation.
 * 
 * @param str 
 * @return 
 */
int	ft_atoi(const char *str)
{
	int		res;
	size_t	i;
	int		sign;

	res = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + str[i] - 48;
		i++;
	}
	return (res * sign);
}
