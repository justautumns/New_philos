/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/29 15:29:41 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	am_i_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if ((int)(get_time() - philo->last_meal) >= philo->data->time_to_die)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), 1);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

static int	eating2(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (!philo->data->dead_flag && !philo->data->eat_enough)
		printings(philo, "is eating");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->print_mutex);
	my_usleep(philo->data->time_to_eat, philo);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->how_many_times_eated++;
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]);
	pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
	return (0);
}

static int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->eat_enough == true || philo->data->dead_flag == true)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), -16);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->forks[philo->forks[0]]);
	if (am_i_dead(philo) == 1)
		return (pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]),
			-16);
	printings(philo, "has taken a fork");
	if (philo->data->number_of_philos == 1)
	{
		my_usleep(philo->data->time_to_die, philo);
		pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
		return (printings(philo, "died"), -16);
	}
	pthread_mutex_lock(&philo->data->forks[philo->forks[1]]);
	if (am_i_dead(philo) == 1)
		return (pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]),
			pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]), -16);
	printings(philo, "has taken a fork");
	eating2(philo);
	return (0);
}

static int	sleeping_thinking(t_philo *philo)
{
	time_t	think;

	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->eat_enough == true || philo->data->dead_flag == true)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), -15);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	think = ((philo->data->time_to_die - (philo->data->time_to_sleep
					+ philo->data->time_to_eat)) / 2);
	if (think <= 0)
		think = 1;
	if (am_i_dead(philo) == 1)
		return (-15);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (!philo->data->dead_flag && !philo->data->eat_enough)
		printings(philo, "is sleeping");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	my_usleep(philo->data->time_to_sleep, philo);
	if (am_i_dead(philo) == 1)
		return (-15);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (!philo->data->dead_flag && !philo->data->eat_enough)
		printings(philo, "is thinking");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	my_usleep(think, philo);
	return (0);
}

void	*philo_routines(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	pthread_mutex_lock(&philo->data->dead_mutex);
	philo->last_meal = philo->data->begin_time;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (philo->philo_id % 2)
		my_usleep(100, NULL);
	while (1)
	{
		if (eating(philo) == -16)
			break ;
		if (sleeping_thinking(philo) == -15)
			break ;
	}
	return (NULL);
}
