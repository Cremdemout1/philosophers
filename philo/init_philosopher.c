/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosopher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:14:33 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 19:18:00 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

t_philosopher	*initialize_philosopher(t_table *table, int i)
{
	t_philosopher	*philosopher;

	philosopher = malloc(sizeof(t_philosopher));
	if (!philosopher)
	{
		perror("Error initializing philosopher");
		exit(EXIT_FAILURE);
	}
	philosopher->id = i + 1;
	philosopher->full = 0;
	philosopher->last_meal_time = get_time();
	philosopher->times_eaten = 0;
	philosopher->table = table;
	philosopher->thread = 0;
	if (table->philosopher_num % 2 == 0)
		philosopher->thinking_time = table->time_to_eat - table->time_to_sleep;
	else
	{
		if (philosopher->id % 2 == 0 && table->time_to_eat > table->time_to_sleep)
			philosopher->thinking_time = table->time_to_eat - table->time_to_sleep;
		else
			philosopher->thinking_time = table->time_to_eat * 2 - table->time_to_sleep;
		if (philosopher->thinking_time < 0)
			philosopher->thinking_time = 0;
	}
	
	philosopher->right_fork = malloc(sizeof(t_fork));
	if (!philosopher->right_fork)
	{
		perror("Right fork malloc failed");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_init(&philosopher->right_fork->lock, NULL);
	philosopher->right_fork->locked = false;
	pthread_mutex_init(&philosopher->philo_mutex, NULL);
	philosopher->left_fork = NULL;
	philosopher->next = NULL;
	return (philosopher);
}

t_philosopher	*find_latest_member(t_philosopher *current)
{
	if (!current)
	{
		perror("List is empty");
		exit(EXIT_FAILURE);
	}
	while (current->next)
		current = current->next; 
	return current;
}

void	add_to_group(t_philosopher **group, t_philosopher *new_member)
{
	t_philosopher *last_addition;
	if (!new_member)
	{
		perror("New member not initialized");
		return;
	}
	if (!*group)
		*group = new_member;
	else
	{
		last_addition = find_latest_member(*group);
		last_addition->next = new_member;
	}
}

void	connect_forks(t_philosopher **group)
{
	t_philosopher *current;
	t_philosopher *first;
	t_philosopher *next;

	current = *group;
	first = current;
	if (!group || !*group)
	{
		perror("No group to give forks to");
		exit(1);
	}
	while (current->next)
	{
		next = current->next;
		next->left_fork = current->right_fork;
		current = next;
	}
	first->left_fork = current->right_fork;
}

void	single_philo(t_philosopher *philo)
{
	write_status(FORK1, philo);
	usleep(philo->table->time_to_die * 1000);
	write_status(DEAD, philo);
}

int	create_philosophers(t_table *table, t_philosopher **group)
{
	int i;
	t_philosopher *philosopher;

	i = 0;
	while (i < table->philosopher_num)
	{
		philosopher = initialize_philosopher(table, i);
		add_to_group(group, philosopher);
		i++;
	}
	if (table->philosopher_num == 1)
	{
		single_philo(*group);
		return (1);
	}
	connect_forks(group);
	return (0);	
}

