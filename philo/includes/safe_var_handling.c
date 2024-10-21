/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_var_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:00:07 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 19:11:49 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_int(pthread_mutex_t *mutex, int *value)
{
	int	new;
		
	pthread_mutex_lock(mutex);
	new = *value;
	pthread_mutex_unlock(mutex);
	return (new);
}

void	set_int(pthread_mutex_t *mutex, int *var, int value)
{
	pthread_mutex_lock(mutex);
	*var = value;
	pthread_mutex_unlock(mutex);
}

int get_long(pthread_mutex_t *mutex, long *value)
{
	long new;
		
	pthread_mutex_lock(mutex);
	new = *value;
	pthread_mutex_unlock(mutex);
	return (new);
}

void	set_long(pthread_mutex_t *mutex, long *var, long value)
{
	pthread_mutex_lock(mutex);
	*var = value;
	pthread_mutex_unlock(mutex);
}

void	write_status(t_status status, t_philosopher *philo)
{
	long time_passed;

	pthread_mutex_lock(&philo->table->table_lock);
	time_passed = get_time() - philo->table->start_time;
	if ((status == FORK1 || status == FORK2) && !philo->table->end_experiment)
		printf("%ld %ld has taken a fork\n", time_passed, philo->id);
	else if (status == EATING && !philo->table->end_experiment)
		printf("%ld %ld is eating\n", time_passed, philo->id); 
	else if (status == SLEEPING && !philo->table->end_experiment)
		printf("%ld %ld is sleeping\n", time_passed, philo->id); 
	else if (status == DEAD)
		printf("\033[1;31m%ld %ld died\n\033[0m", time_passed, philo->id);
	else if (status == THINKING && !philo->table->end_experiment)
		printf("%ld %ld is thinking\n", time_passed, philo->id);	
	pthread_mutex_unlock(&philo->table->table_lock);
}
