/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/08/14 23:59:14 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

static int	thinking_part(t_philo *philo)
{
	time_t	think;

	think = ((philo->data->time_to_die - (philo->data->time_to_sleep
					+ philo->data->time_to_eat)) / 2);
	if (think <= 0)
		think = 0;
	else if (think > 600)
		think = 200;
	if (am_i_dead(philo) == 1)
		return (-15);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (!philo->data->dead_flag && !philo->data->eat_enough)
		printings(philo, "is thinking");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (my_usleep(think, philo) == -1)
		return (-1);
	return (0);
}

static int	eating2(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (!philo->data->dead_flag)
		printings(philo, "is eating");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->last_meal = get_time();
	if ((uint64_t)philo->last_meal == TIME_ERROR)
		return (pthread_mutex_unlock(&philo->data->print_mutex),
			pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]),
			pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]),
			-16);
	pthread_mutex_unlock(&philo->data->print_mutex);
	if (my_usleep(philo->data->time_to_eat, philo) == -1)
		return (-16);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->ate_enough)
		philo->how_many_times_eated++;
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]);
	pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
	return (0);
}

static int	eating(t_philo *philo)
{
	if (am_i_done(philo) == 1)
		return (-16);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead_flag == true)
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
	return (eating2(philo), 0);
}

static int	sleeping_thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead_flag == true)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), -15);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (am_i_dead(philo) == 1)
		return (-15);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (!philo->data->dead_flag && !philo->data->eat_enough)
		printings(philo, "is sleeping");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (am_i_done(philo) == 1)
		return (-16);
	if (my_usleep(philo->data->time_to_sleep, philo) == -1)
		return (-15);
	if (thinking_part(philo) == -1)
		return (-15);
	return (0);
}

void	*philo_routines(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead_flag == true)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), NULL);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->last_meal = philo->data->begin_time;
	pthread_mutex_unlock(&philo->data->print_mutex);
	if (philo->philo_id % 2)
		if (my_usleep1(100, philo) == -1)
			return (NULL);
	while (1)
	{
		if (eating(philo) == -16)
			break ;
		if (sleeping_thinking(philo) == -15)
			break ;
	}
	return (NULL);
}
