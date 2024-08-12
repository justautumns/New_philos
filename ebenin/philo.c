/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:47:53 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/08/12 20:26:47 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	freeing(t_data *data)
{
	free(data->philos);
	free(data);
}

void	check(t_data *data)
{
	int	m;

	m = 0;
	printf("%d\n", data->number_of_philos);
	while (m < data->number_of_philos)
	{
		printf("Philo id :%d , how many times ate: %d\n",
			data->philos[m].philo_id,
			data->philos[m].how_many_times_eated);
		m++;
	}
}

void	free_mutexes(t_data *data)
{
	int	k;

	k = 0;
	while (k < data->number_of_philos)
	{
		pthread_mutex_destroy(&data->forks[k]);
		k++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->dead_mutex);
	free(data->forks);
}

int	second_part_of_main(t_data *start)
{
	bool	alright;

	alright = false;
	if (thread_create(start) == -1)
	{
		alright = true;
		ft_error("Thread creation error\n", NULL);
	}
	if (threads_join(start) == -1)
		return (ft_error("Thread/Join Error\n", start), -1);
	free_mutexes(start);
	check(start);
	freeing(start);
	if (alright == true)
		return (-1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*start;

	if (ac < 2 || ac > 6)
		return (printf("Please enter arguments correctly\n"), -1);
	if (!error_check(av))
		return (-1);
	if (av[5] && ft_atoi(av[5]) == 0)
		return (ft_error("Please enter arguments correctly\n", NULL), -1);
	start = init_data(av);
	if (start == NULL)
		return (ft_error("Allocation failed\n", NULL), -1);
	if (init_mutexes(start) == -1)
		return (freeing(start), ft_error("Mutex Error\n", NULL), -1);
	if (second_part_of_main(start) == -1)
		return (-1);
	return (0);
}

