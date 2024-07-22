/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:13:57 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/22 21:39:14 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	ft_atoi(char *str)
{
	unsigned long long	res;
	int					sign;
	int					k;

	res = 0;
	sign = 1;
	k = 0;
	while ((str[k] >= 9 && str[k] <= 13) || str[k] == ' ')
		k++;
	if (str[k] == '-' || str[k] == '+')
	{
		if (str[k] == '-')
			sign = -1;
		k++;
	}
	while (str[k] >= '0' && str[k] <= '9')
	{
		res = res * 10 + (str[k] - 48);
		k++;
	}
	return (res * sign);
}

uint64_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (printf("Time error\n"), -1);
	return ((time.tv_sec * (uint64_t)1000) + time.tv_usec / 1000);
}

void	my_usleep(int difference)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < (uint64_t)difference)
		usleep(100);
}
