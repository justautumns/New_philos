/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:47:53 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 21:18:09 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	check(t_data *data)
{
	int	m;

	m = 0;
	printf("%d\n",data->number_of_philos);
	while (m < data->number_of_philos)
	{
		printf("Philo id :%d , %d\n", data->philos[m]->philo_id, data->philos[m]->how_many_times_eated);
		m++;
	}
}
void	freeing(t_data *data)
{
	int	k;

	k = 0;
	while (k < data->number_of_philos)
	{
		free(data->philos[k]);
		k++;
	}
	free(data->philos);
	free(data);
}
void	free_mutexes(t_data *data)
{
	int	k;

	k = -1;
	while (++k < data->number_of_philos)
		pthread_mutex_destroy(&data->forks[k]);
	free(data->forks);
}
int main(int ac, char **av)
{
	t_data	*start;

	if (ac < 2 || ac > 6)
		return (printf("Please enter arguments correctly\n"), -1);
	if (!error_check(av))
		return (-1);
	start = init_data(av);
	//test(start);
	init_mutexes(start);
	thread_create(start);
	threads_join(start);
	free_mutexes(start);
	check(start);
	freeing(start);
	return (0);
}
