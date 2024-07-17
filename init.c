/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:13:52 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 12:32:47 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void test(t_data *data)
{
	int k = 0;
	t_data *x = data;
	while(k < x->number_of_philos)
	{
		printf("Philo %d : Left spoon %d Right spoon %d\n", x->philos[k]->philo_id, x->philos[k]->left_spoon_no, x->philos[k]->right_spoon_no);
		k++;
	}
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
			return (-1);
		m++;
	}
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (-1);
	return (0);
}
void	init_spoons(t_data *data, t_philo *philo, int m)
{
	if (data->number_of_philos % 2 == 0)
	{
		philo->left_spoon_no = (m + 1) % data->number_of_philos;
		philo->right_spoon_no = m;
	}
	else if (data->number_of_philos % 2 != 0)
	{
		philo->left_spoon_no = m;
		philo->right_spoon_no = (m + 1) % data->number_of_philos;
	}
}
t_philo	**init_philos(t_data *data)
{
	int		m;
	t_philo	**philos;

	philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!philos)
		return (NULL);
	m = 0;
	while (m < data->number_of_philos)
	{
		philos[m] = malloc(sizeof(t_philo) * 1);
		if (!philos[m])
			return (NULL);
		philos[m]->data = data;
		philos[m]->how_many_times_eated = 0;
		philos[m]->last_meal = 0;
		philos[m]->philo_id = m + 1;
		init_spoons(data, philos[m], m);
	m++;
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
	my_data->philos = init_philos(my_data);
	return (my_data);
}
