/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:24:12 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/30 22:43:01 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	*test_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	pthread_mutex_lock(&philo->data->dead_mutex);
	usleep(1000);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (NULL);
}

int	thread_test(t_data *data)
{
	int	m;
	int	k;

	m = 0;
	while (m < data->number_of_philos)
	{
		if (pthread_create(&data->philos[m].thread, NULL,
				&test_routine, &data->philos[m]) != 0)
			break ;
		m++;
	}
	if (m != data->number_of_philos)
		data->f_something_happens = true;
	k = 0;
	while (k < m)
	{
		if (pthread_join(data->philos[k].thread, NULL) != 0)
			return (-1);
		k++;
	}
	return (0);
}

int	thread_create(t_data *data)
{
	int		m;

	if (data->f_something_happens == true)
		return (-1);
	m = 0;
	data->begin_time = get_time();
	if (data->begin_time == -1)
		return (-1);
	while (m < data->number_of_philos)
	{
		// data->f_something_happens = true;
		if (pthread_create(&data->philos[m].thread, NULL,
				&philo_routines, &data->philos[m]) != 0)
			{
			while (m > 0)
			{
				m--;
				pthread_join(data->philos[m].thread, NULL);
			}
			return (-1);
			}
		// if (m == 4)
		// 	break ;
		m++;
	}
	if (data->number_of_philos > 1)
	{
		if (pthread_create(&data->monitor, NULL, &doch_sauron, data) != 0)
			return (-1);
	}
	return (0);
}

int	threads_join(t_data *data)
{
	int	m;

	m = 0;
	while (m < data->number_of_philos)
	{
		if (pthread_join(data->philos[m].thread, NULL) != 0)
			return (-1);
		m++;
	}
	if (data->number_of_philos > 1)
	{
		if (pthread_join(data->monitor, NULL) != 0)
			return (-1);
	}
	return (0);
}
