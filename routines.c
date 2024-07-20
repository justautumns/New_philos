/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:24:55 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/20 20:15:16 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

static void	printings(t_philo *philo, char *str)
{
	// if (philo->data->dead_flag == true)
	// 	return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_time() - philo->data->begin_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

bool	check_bakalim(t_philo *philo)
{
	time_t	time;

	time = get_time();
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings)
		return (pthread_mutex_unlock(&philo->data->print_mutex), true); // should be false but need to check if all philos are full; although it seems to be working don't know why
	//printf("time passed for %d: %ld\n", philo->philo_id, (time - philo->last_meal));
	if ((time - philo->last_meal) >= philo->data->time_to_die) // changed > to >=
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_lock(&philo->data->dead_mutex); // added ?
		philo->data->dead_flag = true;
		pthread_mutex_unlock(&philo->data->dead_mutex);
		printf("%ld %d died\n", get_time() - philo->data->begin_time, philo->philo_id);
		return (true);
	}
	return (pthread_mutex_unlock(&philo->data->print_mutex), false);
}

void *doch_sauron(void	*pointer)
{
	t_data *data;
	int		m;

	data = (t_data *)pointer;
	m = 0;
	my_usleep(1); //added
	while (m < data->number_of_philos)
	{
		// pthread_mutex_lock(&data->print_mutex);
		if (check_bakalim(data->philos[m]) == true)
		{
			//pthread_mutex_unlock(&data->print_mutex);
			// pthread_mutex_lock(&data->dead_mutex); // added ?
			// data->philos[m]->data->dead_flag = true;
			// pthread_mutex_unlock(&data->dead_mutex);
			break ;
		}
		//pthread_mutex_unlock(&data->print_mutex);
		m++;
		if (m == data->number_of_philos)
			m = 0;
		//my_usleep(1000); // the value here is time of death in some cases, but why?
	}
	//pthread_mutex_unlock(&data->print_mutex);
	return (NULL);
}

int	check_dead_flag(t_philo *philo) // added
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	// if (philo->data->dead_flag == true)
	if ((int)(get_time() - philo->last_meal) >= philo->data->time_to_die)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), 1);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

static int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex); // added
	// if (philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings && philo->data->dead_flag == true)
	// 	return (pthread_mutex_unlock(&philo->data->dead_mutex), 0);
	if ((philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings) || philo->data->dead_flag == true)
		return (pthread_mutex_unlock(&philo->data->dead_mutex), -69);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->forks[philo->forks[0]]);
	if (check_dead_flag(philo) == 1)
		return (pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]), -69);
	printings(philo, "has taken a fork");
	if (philo->data->number_of_philos == 1)
	{
		my_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]);
		return (printings(philo, "died"), -69);
	}
	pthread_mutex_lock(&philo->data->forks[philo->forks[1]]);
	if (check_dead_flag(philo) == 1) // added
		return (pthread_mutex_unlock(&philo->data->forks[philo->forks[1]]),
			pthread_mutex_unlock(&philo->data->forks[philo->forks[0]]), -69);
	printings(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->dead_mutex); // added
	if (!philo->data->dead_flag)
		printings(philo, "is eating");
	pthread_mutex_unlock(&philo->data->dead_mutex);
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
	if (check_dead_flag(philo) == 1) // added
		return (-1);
	pthread_mutex_lock(&philo->data->dead_mutex); // added (if check above stays, the next check with if, is not necessary)
	if (!philo->data->dead_flag)
		printings(philo, "is sleeping");
	pthread_mutex_unlock(&philo->data->dead_mutex);
	my_usleep(philo->data->time_to_sleep);
	if (check_dead_flag(philo) == 1) // added (wrong philo is dying, but this check makes a philo die before it starts to think past his time to die)
		return (-1);
	pthread_mutex_lock(&philo->data->dead_mutex); // added
	if (!philo->data->dead_flag)
		printings(philo, "is thinking");
	pthread_mutex_unlock(&philo->data->dead_mutex);
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
	if (philo->philo_id % 2 == 0)
	{
		printings(philo, "is thinking");
		my_usleep(100);
	}
	while (1) // changed condition
	{
		if ((philo->data->number_of_eatings != 0 && philo->how_many_times_eated >= philo->data->number_of_eatings))
		{
			break ;
		}
		if (eating(philo) == -69)
		{
			break;
		}
		sleeping_thinking(philo);
		pthread_mutex_lock(&philo->data->dead_mutex); // added
		if (philo->data->dead_flag == true)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
	}
	return (NULL);
}

