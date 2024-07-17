/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:24:12 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 13:43:19 by mehmeyil         ###   ########.fr       */
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
void	thread_create(t_data *data)
{
	t_data	*pointer;
	int		m;
	
	pointer = data;
	m = 0;
	data->begin_time = get_time();
	while (m < pointer->number_of_philos)
	{
		if (pthread_create(&pointer->philos[m]->thread, NULL, &philo_routines, pointer) != 0)
			return ;
		m++;
	}
	if (pointer->number_of_philos > 1)
	{
		if (pthread_create(&pointer->monitor, NULL, &doch_sauron, pointer) != 0)
			return ;
	}
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
	pthread_join(data->monitor, NULL);
}
