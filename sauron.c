/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sauron.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 21:15:56 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/30 22:31:32 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	printings(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_time() - philo->data->begin_time,
		philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

bool	check_bakalim(t_philo *philo)
{
	time_t	time;

	time = get_time();
	pthread_mutex_lock(&philo->data->print_mutex);
	if ((time - philo->last_meal) >= philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->dead_mutex);
		philo->data->dead_flag = true;
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld %d died\n", get_time() - philo->data->begin_time,
			philo->philo_id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (true);
	}
	return (pthread_mutex_unlock(&philo->data->print_mutex), false);
}

bool	hungry_or_not(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated
		> philo->data->number_of_eatings)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->dead_mutex);
		philo->data->eat_enough = true;
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (false);
}

void	*doch_sauron(void	*pointer)
{
	t_data	*data;
	int		m;

	data = (t_data *)pointer;
	m = 0;
	pthread_mutex_lock(&data->dead_mutex);
	if (data->f_something_happens == true)
		return(pthread_mutex_unlock(&data->dead_mutex), NULL);
	pthread_mutex_unlock(&data->dead_mutex);
	my_usleep(data->time_to_die, NULL);
	while (m < data->number_of_philos)
	{
		if (check_bakalim(&data->philos[m]) == true)
			break ;
		else if (hungry_or_not(&data->philos[m]) == true)
			break ;
		m++;
		if (m == data->number_of_philos)
			m = 0;
		usleep(1000);
	}
	return (NULL);
}
