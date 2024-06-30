/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmeyil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:47:53 by mehmeyil          #+#    #+#             */
/*   Updated: 2024/06/30 23:28:33 by mehmeyil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int main(int ac, char **av)
{
	t_data *start;
	
	if (ac < 2 || ac > 6)
		return (printf("Please enter arguments correctly\n"), -1);
	if (!error_check(av))
		return (-1);
	start = init(av);
	mutex_destroying(start);
	free_all(start);
	return (0);
}
