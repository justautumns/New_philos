/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:47:53 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 15:14:59 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

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
	freeing(start);
	return (0);
}
