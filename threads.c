/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 17:00:25 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/06/30 23:34:39 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	print_the_action(int action, t_philo *philos)
{
	t_philo	*pointer;

	pointer = philos;
	pthread_mutex_lock(&pointer->data->f_print);
	if (action == 0)
		printf("%lu %d has taken a fork\n", get_time() - pointer->data->begin_time, pointer->philo_id);
	if (action == 1)
		printf("%lu %d is eating\n", get_time() - pointer->data->begin_time, pointer->philo_id);
	if (action == 2)
		printf("%lu %d is sleeping\n", get_time() - pointer->data->begin_time, pointer->philo_id);
	if (action == 3)
		printf("%lu %d is thinking\n", get_time() - pointer->data->begin_time, pointer->philo_id);
	pthread_mutex_unlock(&pointer->data->f_print);
}

void	*inadina_sauron(void *arg)
{
	t_data	*pointer;
	int		k;

	pointer = (t_data *)arg;
	k = 0;
	while (!pointer->dead_flag)
	{
		while (k < pointer->number_of_philos)
		{
			if (get_time() - pointer->philos[k].last_meal > (uint64_t)pointer->time_to_die)
			{
				pthread_mutex_lock(&pointer->f_print);
				printf("%lu %d Died\n", get_time() - pointer->begin_time, pointer->philos[k].philo_id);
				pointer->dead_flag = true;
				pthread_mutex_unlock(&pointer->f_print);
				return (NULL);
			}
		k++;
		}
		usleep(1000);
	}
	return (NULL);
}
void	*routine(void *arg)
{
	t_philo	*pointer;

	pointer = (t_philo *)arg;
	while (!pointer->data->dead_flag)
	{
		pthread_mutex_lock(&pointer->data->forks[pointer->left_fork_no]);
		print_the_action(0, pointer);
		pthread_mutex_lock(&pointer->data->forks[pointer->right_fork_no]);
		print_the_action(0, pointer);
		print_the_action(1, pointer);
		pointer->last_meal = get_time();
		my_usleep(pointer->data->time_to_eat);
		pointer->how_many_times_eated++;
		pthread_mutex_unlock(&pointer->data->forks[pointer->left_fork_no]);
		pthread_mutex_unlock(&pointer->data->forks[pointer->right_fork_no]);
		print_the_action(2, pointer);
		my_usleep(pointer->data->time_to_sleep);
		print_the_action(3, pointer);
	}
	return (NULL);
}
