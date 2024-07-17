/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:38:23 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/07/17 15:18:51 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <stdint.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_philo t_philo;

typedef struct s_data
{
	time_t			begin_time;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				number_of_philos;
	int				number_of_eatings;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_mutex;
	t_philo			**philos;
	pthread_t		monitor;
	bool			dead_flag;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				philo_id;
	int				how_many_times_eated;
	int				left_spoon_no;
	int				right_spoon_no;
	time_t			last_meal;
	struct s_data	*data;
}	t_philo;

// Utils
int			ft_atoi(char *str);
uint64_t	get_time();
bool		error_check(char **str);
void		*ft_error(char *str, t_data *data);
void		my_usleep(int difference);
void		mutex_destroying(t_data *data);
void		free_all(t_data *data);

// INIT
t_data		*init_data(char **av);
int			init_mutexes(t_data *data);

// TESTS
void	test(t_data *data);

// BEGINNING THE PROGRAMM
void 	*doch_sauron(void	*pointer);
void	*philo_routines(void *pointer);
void	thread_create(t_data *data);
void	threads_join(t_data *data);
#endif
