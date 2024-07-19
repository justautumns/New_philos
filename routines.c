/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/20 01:07:00 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

static void	printings(t_philo *philo, char *str)
{
	if (philo->data->dead_flag == true)
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_time() - philo->data->begin_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

bool	check_bakalim(t_philo *philo)
{
	time_t	time;

	time = get_time();
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
		return (false);
	
	if ((time - philo->last_meal) > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		printf("%ld %d died\n", get_time() - philo->data->begin_time, philo->philo_id);
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (true);
	}
	return (false);
}

void	is_time_delay(time_t k)
{
	while (get_time() < (uint64_t)k)
		continue;
}
void *doch_sauron(void	*pointer)
{
	t_data *data;
	int		m;

	data = (t_data *)pointer;
	m = 0;
	is_time_delay(data->begin_time);
	while (m < data->number_of_philos)
	{
		pthread_mutex_lock(&data->print_mutex);
		if (check_bakalim(data->philos[m]) == true)
		{
			data->dead_flag = true;
			pthread_mutex_unlock(&data->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->print_mutex);
		m++;
		my_usleep(1000);
	}
	//pthread_mutex_unlock(&data->print_mutex);
	return (NULL);
}

static int	eating(t_philo *philo)
{
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings && philo->data->dead_flag == true)
		return (0);
	pthread_mutex_lock(&philo->data->forks[philo->forks[0]]);
	printings(philo, "has taken a fork");
	if (philo->data->number_of_philos == 1)
	{
		my_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
		return (printings(philo, "died"), -69);
	}
	pthread_mutex_lock(&philo->data->forks[philo->forks[1]]);
	printings(philo, "has taken a fork");
	if (!philo->data->dead_flag)
		printings(philo, "is eating");
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->print_mutex);
	my_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->print_mutex);
	philo->how_many_times_eated++;
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]);
	pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
	return (0);
}

static int	sleeping_thinking(t_philo *philo)
{
	time_t	think;

	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
		return (0);
	think = (((get_time() - philo->last_meal) - philo->data->time_to_sleep) - 60);
	if (think <= 0)
		think = 1;
	if (!philo->data->dead_flag)
		printings(philo, "is sleeping");
	my_usleep(philo->data->time_to_sleep);
	if (!philo->data->dead_flag)
		printings(philo, "is thinking");
	my_usleep(think);
	
	return (0);
}
void	*philo_routines(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;

	//printf("%d\n",philo->data->number_of_eatings);
	pthread_mutex_lock(&philo->data->dead_mutex);
	philo->last_meal = philo->data->begin_time;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	is_time_delay(philo->data->begin_time);
	 if (philo->philo_id % 2 == 0)
	{
		printings(philo, "is thinking");
		my_usleep(100);
	}
	while (1)
	{
		if ((philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings) || philo->data->dead_flag == true)
		{
			break ;
		}
		if (eating(philo) == -69)
		{
			break;
		}
		sleeping_thinking(philo);
	}
	return (NULL);
}
