/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:13:57 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/08/18 18:53:15 by mehmeyil         ###   ########.fr       */
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
		return (ft_error("Time error occured\n", NULL), TIME_ERROR);
	return ((time.tv_sec * (uint64_t)1000) + time.tv_usec / 1000);
}

int	my_usleep1(int difference, t_philo *philo)
{
	time_t	start;

	start = get_time();
	if ((uint64_t)start == TIME_ERROR)
		return (-1);
	while ((get_time() - start) < (uint64_t)difference)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		if (philo->data->number_of_philos == 1)
		{
			pthread_mutex_unlock(&philo->data->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->print_mutex);
		usleep(100);
	}
	return (0);
}

int	my_usleep(int difference, t_philo *philo)
{
	time_t	start;

	start = get_time();
	if ((uint64_t)start == TIME_ERROR)
		return (-1);
	if (philo == NULL)
	{
		while ((get_time() - start) < (uint64_t)difference)
			usleep(100);
	}
	else
	{
		while ((get_time() - start) < (uint64_t)difference)
		{
			pthread_mutex_lock(&philo->data->dead_mutex);
			if (philo->data->dead_flag == true)
			{
				pthread_mutex_unlock(&philo->data->dead_mutex);
				break ;
			}
			pthread_mutex_unlock(&philo->data->dead_mutex);
			usleep(100);
		}
	}
	return (0);
}

int	am_i_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if ((int)(get_time() - philo->last_meal) >= philo->data->time_to_die)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), 1);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}
