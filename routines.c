/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/18 15:26:06 by mehmeyil         ###   ########.fr       */
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
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
		return (false);
	time_t	time;

	time = get_time();
	if ((time - philo->last_meal) > philo->data->time_to_die)
	{
		printf("%ld %d died\n", get_time() - philo->data->begin_time, philo->philo_id);
		philo->data->dead_flag = true;
		return (true);
	}
	return (false);
}
void *doch_sauron(void	*pointer)
{
	t_data *data;
	int		m;

	data = (t_data *)pointer;
	m = 0;
	while (m < data->number_of_philos)
	{
		pthread_mutex_lock(&data->print_mutex);
		if (check_bakalim(data->philos[m]) == true)
			break ;
		pthread_mutex_unlock(&data->print_mutex);
		m++;
		my_usleep(1000);
	}
	pthread_mutex_unlock(&data->print_mutex);
	return (NULL);
}

static int	eating(t_philo *philo)
{
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
		return (0);
	pthread_mutex_lock(&philo->data->forks[philo->left_spoon_no]);
	printings(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[philo->right_spoon_no]);
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
	pthread_mutex_unlock(&philo->data->forks[philo->left_spoon_no]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_spoon_no]);
	return (0);
}

static int	sleeping_thinking(t_philo *philo)
{
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
		return (0);
	if (!philo->data->dead_flag)
		printings(philo, "is sleeping");
	my_usleep(philo->data->time_to_sleep);
	if (!philo->data->dead_flag)
		printings(philo, "is thinking");
	my_usleep(philo->data->time_to_die - (philo->data->time_to_eat + philo->data->time_to_sleep));
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
	 if (philo->philo_id % 2 == 0)
		my_usleep(100);
	while (1)
	{
		if ((philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings) || philo->data->dead_flag == true)
			break ;
		eating(philo);
		sleeping_thinking(philo);
	}
	return (NULL);
}
