/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:38:11 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/06/30 23:27:15 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	philos_init(t_data *data)
{
	int	m;
	data->philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!data->philos)
		return ;
	m = 0;
	while (m < data->number_of_philos)
	{
		data->philos[m].philo_id = m + 1;
		data->philos[m].how_many_times_eated = 0;
		data->philos[m].last_meal = get_time();
		data->philos[m].data = data;
		data->philos[m].left_fork_no = m;
		data->philos[m].right_fork_no = (m + 1) % data->number_of_philos;
	}
}

void	threads_and_mutexes(t_data *data)
{
	int		k;

	k = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philos);
	if (!data->forks)
		return ;
	pthread_mutex_init(&data->f_print, NULL);
	pthread_create(&data->monitor, NULL, &inadina_sauron, data);
	while (k < data->number_of_philos)
	{
		pthread_mutex_init(&data->forks[k], NULL);
		pthread_create(&data->philos[k].thread, NULL, &routine, &data->philos[k]);
		k++;
	}
	k = 0;
	while(k < data->number_of_philos)
	{
		pthread_join(data->philos[k].thread, NULL);
		k++;
	}
	pthread_join(data->monitor, NULL);
}
t_data	*init(char **av)
{
	t_data *data;

	data = malloc(sizeof(t_data) * 1);
	if (!data)
		return (NULL);
	data->number_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->dead_flag = false;
	if (av[5])
		data->number_of_eatings = ft_atoi(av[5]);
	else
		data->number_of_eatings = -1;
	data->begin_time = get_time();
	philos_init(data);
	threads_and_mutexes(data);
	return (data);
}
