/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorchecks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 17:03:04 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/20 18:58:47 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	ft_error(char *str, t_data *data)
{
	if (data != NULL)
	{
		freeing(data);
		free_mutexes(data);
	}
	while (*str)
		write(2, str++, 1);
}
static bool	input_check(char *str)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}
static bool	overflow_check(char *str)
{
	int	m;

	m = 0;
	while (str[m])
		m++;
	if (m >= 10 && str[0] > '2')
			return (false);
	return (true);
}
bool	error_check(char **str)
{
	if (!str[1] || !str[2] || !str[3] || !str[4])
		return (ft_error("Please enter in correct order\n", NULL), false);
	if (ft_atoi(str[1]) > 200 || ft_atoi(str[1]) <= 0
		|| !input_check(str[1]) || !overflow_check(str[1]))
		return (ft_error("Error: Incorrect philo value\n", NULL), false);
	if (ft_atoi(str[2]) <= 0 || !input_check(str[2])
		|| ft_atoi(str[2]) > INT_MAX || !overflow_check(str[2]))
		return (ft_error("Error: Incorrect time to die value\n", NULL), false);
	if (ft_atoi(str[3]) <= 0 || !input_check(str[3])
		|| ft_atoi(str[3]) > INT_MAX || !overflow_check(str[3]))
		return (ft_error("Error: Incorrect time to eat value\n", NULL), false);
	if (ft_atoi(str[4]) <= 0 || !input_check(str[4])
		|| ft_atoi(str[4]) > INT_MAX || !overflow_check(str[4]))
		return (ft_error("Error: Incorrect time to sleep value\n", NULL), false);
	if ((str[5]) && ((ft_atoi(str[5]) < 0 || !input_check(str[5])
		|| ft_atoi(str[5]) > INT_MAX) || !overflow_check(str[5])))
		return (ft_error("Error: Incorrect number of time to eat value\n", NULL), false);
	return (true);
}
