/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:13:52 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/31 17:47:57 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	mutex_what_if(t_data *data, int m)
{
	while (m > 0)
	{
		m--;
		pthread_mutex_destroy(&data->forks[m]);
	}
	free (data->forks);
}

int	init_mutexes(t_data *data)
{
	int	m;

	m = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philos);
	if (!data->forks)
		return (-1);
	while (m < data->number_of_philos)
	{
		if (pthread_mutex_init(&data->forks[m], NULL) != 0)
		{
			while (m > 0)
			{
				m--;
				pthread_mutex_destroy(&data->forks[m]);
			}
			return (free(data->forks), -1);
		}
		m++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (mutex_what_if(data, m), -1);
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
		return (mutex_what_if(data, m),
			pthread_mutex_destroy(&data->print_mutex), -1);
	return (0);
}

void	init_spoons(t_data *data, t_philo *philo)
{
	philo->forks[0] = philo->philo_id - 1;
	philo->forks[1] = (philo->philo_id) % data->number_of_philos;
	if (philo->philo_id % 2)
	{
		philo->forks[0] = (philo->philo_id) % data->number_of_philos;
		philo->forks[1] = philo->philo_id - 1;
	}
}

t_philo	*init_philos(t_data *data)
{
	int		m;
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!philos)
		return (NULL);
	m = -1;
	while (++m < data->number_of_philos)
	{
		philos[m].data = data;
		philos[m].how_many_times_eated = 0;
		philos[m].last_meal = 0;
		philos[m].philo_id = m + 1;
		init_spoons(data, &philos[m]);
	}
	return (philos);
}

t_data	*init_data(char **av)
{
	t_data	*my_data;

	my_data = malloc(sizeof(t_data) * 1);
	if (!my_data)
		return (NULL);
	my_data->number_of_philos = ft_atoi(av[1]);
	my_data->time_to_die = ft_atoi(av[2]);
	my_data->time_to_eat = ft_atoi(av[3]);
	my_data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		my_data->number_of_eatings = ft_atoi(av[5]);
	else
		my_data->number_of_eatings = 0;
	my_data->dead_flag = false;
	my_data->eat_enough = false;
	my_data->f_something_happens = false;
	my_data->philos = init_philos(my_data);
	if (my_data->philos == NULL)
	{
		free(my_data);
		return (NULL);
	}
	return (my_data);
}
