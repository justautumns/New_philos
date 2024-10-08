/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <mehmeyil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:38:23 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/08/14 15:23:57 by mehmeyil         ###   ########.fr       */
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

# define TIME_ERROR UINT64_MAX

typedef struct s_philo	t_philo;

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
	t_philo			*philos;
	pthread_t		monitor;
	bool			dead_flag;
	bool			eat_enough;
	bool			f_something_happens;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				philo_id;
	int				how_many_times_eated;
	int				forks[2];
	bool			ate_enough;
	time_t			last_meal;
	struct s_data	*data;
}	t_philo;

// Utils
int			ft_atoi(char *str);
uint64_t	get_time(void);
bool		error_check(char **str);
void		ft_error(char *str, t_data *data);
int			my_usleep1(int difference, t_philo *philo);
int			my_usleep(int difference, t_philo *philo);
void		freeing(t_data *data);
void		free_mutexes(t_data *data);
void		*ft_calloc(size_t nmemb, size_t size);

// INIT
t_data		*init_data(char **av);
int			init_mutexes(t_data *data);

// TESTS
//void	test(t_data *data);
//int			thread_test(t_data *data);

// BEGINNING THE PROGRAMM
void		*doch_sauron(void *pointer);
void		*philo_routines(void *pointer);
void		printings(t_philo *philo, char *str);
int			thread_create(t_data *data);
int			threads_join(t_data *data);
int			am_i_dead(t_philo *philo);
int			am_i_done(t_philo *philo);
#endif
