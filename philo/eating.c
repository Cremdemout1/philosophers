/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:57:51 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 19:03:29 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

void	grab_forks(t_philosopher *philo)
{
	while (!is_dead(philo))
	{
		pthread_mutex_lock(&philo->left_fork->lock);
		if (philo->left_fork->locked == false)
		{
			philo->left_fork->locked = true;
			pthread_mutex_unlock(&philo->left_fork->lock);
			write_status(FORK1, philo);
			break ;
		}
		pthread_mutex_unlock(&philo->left_fork->lock);
	}
	while (!is_dead(philo))
	{
		pthread_mutex_lock(&philo->right_fork->lock);
		if (philo->right_fork->locked == false)
		{
			philo->right_fork->locked = true;
			pthread_mutex_unlock(&philo->right_fork->lock);
			write_status(FORK1, philo);
			break ;
		}
		pthread_mutex_unlock(&philo->right_fork->lock);
	}
}

void	drop_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork->lock);
	philo->left_fork->locked = false;
	pthread_mutex_unlock(&philo->left_fork->lock);
	pthread_mutex_lock(&philo->right_fork->lock);
	philo->right_fork->locked = false;
	pthread_mutex_unlock(&philo->right_fork->lock);
}

void eat(t_philosopher *philo)
{
	grab_forks(philo);
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time());
	pthread_mutex_lock(&philo->philo_mutex);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->philo_mutex);
	write_status(EATING, philo);
	_usleep(philo->table->time_to_eat, philo);
	if (philo->table->number_of_times_to_eat > 0 && philo->times_eaten >= philo->table->number_of_times_to_eat)
		set_int(&philo->philo_mutex, &philo->full, 1);
	drop_forks(philo);
}