/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 13:48:04 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"


void *doch_sauron(void	*pointer)
{
	t_data *data;
	int		m;

	data = (t_data *)pointer;

	while (!data->dead_flag)
	{
		m = 0;
		while (m < data->number_of_philos)
		{
			pthread_mutex_lock(&data->print_mutex);
			if (get_time() - data->philos[m]->last_meal > (unsigned long)data->time_to_die)
			{
				printf("%ld %d died\n", get_time() - data->begin_time, data->philos[m]->philo_id);
				data->dead_flag = true;
				pthread_mutex_unlock(&data->print_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->print_mutex);
		m++;
		}
		my_usleep(1000);
	}
	return (NULL);
}
static void	printings(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_time() - philo->data->begin_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_spoon_no]);
	printings(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[philo->right_spoon_no]);
	printings(philo, "has taken a fork");
	printings(philo, "is eating");
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->print_mutex);
	my_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->how_many_times_eated++;
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->forks[philo->left_spoon_no]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_spoon_no]);
}

static void	sleeping_thinking(t_philo *philo)
{
	printings(philo, "is sleeping");
	my_usleep(philo->data->time_to_sleep);
	printings(philo, "is thinking");
}
void	*philo_routines(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;

	philo->last_meal = philo->data->begin_time;
	if (philo->philo_id % 2 == 0)
		my_usleep(100);
	while (!philo->data->dead_flag == false)
	{
		eating(philo);
		if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
			break ;
		sleeping_thinking(philo);
	}
	return (NULL);
}
