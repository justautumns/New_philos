/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:13:57 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/06/30 23:29:03 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	ft_atoi(char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	result = 0;
	sign = 1;
	if (!str)
		return (-1);
	while(str[i] && ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32)))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	return (result * sign);
}
uint64_t	get_time()
{
	struct timeval time;

	if (gettimeofday(&time, NULL) == -1)
		return(printf("Time error\n"), -1);
	return((time.tv_sec * (uint64_t)1000) + time.tv_usec/1000);
}
void	my_usleep(int difference)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < (uint64_t)difference)
		usleep(100);
}
void	mutex_destroying(t_data *data)
{
	t_data	*pointer;
	int		k;

	k = 0;
	pointer = data;
	pthread_mutex_destroy(&pointer->f_print);
	while (k < pointer->number_of_philos)
	{
		pthread_mutex_destroy(&pointer->forks[k]);
		k++;
	}
}
void	free_all(t_data *data)
{
	t_data	*pointer;
	int		k;

	pointer = data;
	while (k < pointer->number_of_philos)
	{
		free(&pointer->philos[k]);
		free(&pointer->forks[k]);
		k++;
	}
	free(data);
}
