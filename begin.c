/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:24:12 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/22 21:21:57 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	*test_routine(void *pointer)
{
	t_data	*data;

	data = (t_data *)pointer;
	pthread_mutex_lock(&data->dead_mutex);
	printf("deneme \n");
	my_usleep(1000);
	pthread_mutex_unlock(&data->dead_mutex);
	return (NULL);
}

int	thread_create(t_data *data)
{
	int		m;
	int		k;

	m = 0;
	k = 1;
	data->begin_time = get_time();
	while (m < data->number_of_philos)
	{
		if (pthread_create(&data->philos[m]->thread, NULL,
				&philo_routines, data->philos[m]) != 0)
			return (-1);
		m++;
	}
	if (data->number_of_philos > 1)
	{
		if (pthread_create(&data->monitor, NULL, &doch_sauron, data) != 0)
			return (-1);
	}
	return (0);
}

void	threads_join(t_data *data)
{
	int	m;

	m = 0;
	while (m < data->number_of_philos)
	{
		pthread_join(data->philos[m]->thread, NULL);
		m++;
	}
	if (data->number_of_philos)
		pthread_join(data->monitor, NULL);
}
