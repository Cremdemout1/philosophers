/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 11:49:50 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/05 12:43:26 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sim_end(t_table *table)
{
	return (get_int(&table->table_lock, &table->end_experiment));
}

int	is_dead(t_philosopher *philo)
{
	long	time_passed;
	long	time_to_die;

	if (get_int(&philo->philo_mutex, &philo->full))
		return (0);
	time_passed = get_time()
		- get_long(&philo->philo_mutex, &philo->last_meal_time);
	time_to_die = philo->table->time_to_die;
	if (time_passed >= time_to_die)
		return (1);
	return (0);
}

void	_usleep(unsigned long long time, t_philosopher *philo)
{
	unsigned long	start;

	start = get_time();
	while (get_time() - start < time)
	{
		if (is_dead(philo))
			return ;
		usleep(10);
	}
}

int	check_if_full(t_philosopher *philo)
{
	int	required_meals;

	required_meals = philo->table->number_of_times_to_eat;
	if (required_meals >= 0 && philo->times_eaten == required_meals)
	{
		printf("\033[1;32m%ld %ld is full\033[0m\n",
			get_time() - philo->table->start_time, philo->id);
		return (1);
	}
	return (0);
}

void eat(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork->lock);
		write_status(FORK1, philo);
		pthread_mutex_lock(&philo->left_fork->lock);
		write_status(FORK2, philo);
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->lock);
		write_status(FORK1, philo);
		pthread_mutex_lock(&philo->right_fork->lock);
		write_status(FORK2, philo);
	}
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time());
	pthread_mutex_lock(&philo->philo_mutex);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->philo_mutex);
	write_status(EATING, philo);
	_usleep(philo->table->time_to_eat, philo);
	if (philo->table->number_of_times_to_eat > 0 && philo->times_eaten >= philo->table->number_of_times_to_eat)
		set_int(&philo->philo_mutex, &philo->full, 1);
	pthread_mutex_unlock(&philo->left_fork->lock);
	pthread_mutex_unlock(&philo->right_fork->lock);
}