/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:54:05 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 19:10:19 by ycantin          ###   ########.fr       */
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
		usleep(500);
	}
}

int	check_if_full(t_philosopher *philo)
{
	int	required_meals;

	required_meals = philo->table->number_of_times_to_eat;
	if (required_meals >= 0 && philo->times_eaten == required_meals)
		return (1);
	return (0);
}

long	ft_atol(const char *str)
{
	long	i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] <= 57 && str[i] >= 48)
		result = (result * 10) + (str[i++] - 48);
	return (result * sign);
}