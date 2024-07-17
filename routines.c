/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 23:00:43 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

static void	printings(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_time() - philo->data->begin_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	check_bakalim(t_philo *philo)
{
	time_t	time;

	time = get_time();
	pthread_mutex_lock(&philo->data->dead_mutex);
	if ((time - philo->last_meal) > philo->data->time_to_die)
	{
		philo->data->dead_flag = true;
		printings(philo, "died");
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}
void *doch_sauron(void	*pointer)
{
	t_data *data;
	int		m;

	data = (t_data *)pointer;
	m = 0;
	while (m < data->number_of_philos)
	{
		//pthread_mutex_lock(&data->print_mutex);
		if (check_bakalim(data->philos[m]) == -1)
			break ;
		//pthread_mutex_unlock(&data->print_mutex);
		m++;
		my_usleep(1000);
	}
	return (NULL);
}

static int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_spoon_no]);
	printings(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[philo->right_spoon_no]);
	printings(philo, "has taken a fork");
	if (check_bakalim(philo) != -1)
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
	if (check_bakalim(philo) != -1)
		printings(philo, "is sleeping");
	my_usleep(philo->data->time_to_sleep);
	if (check_bakalim(philo) != -1)
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
	while (philo->data->dead_flag == false)
	{
		eating(philo);
		if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
			break ;
		sleeping_thinking(philo);
	}
	return (NULL);
}
