/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:51:44 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 19:16:12 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

void init_experiment(t_table *table)
{
	t_philosopher *philo;

	if (pthread_mutex_init(&table->table_lock, NULL) != 0
        || pthread_mutex_init(&table->write_lock, NULL) != 0)
            return ;
	philo = NULL;
	if (create_philosophers(table, &philo) == 0)
	{
		if (!philo)
            return ;
		table->philo_list = philo;
		initiate_threads(&philo);
		pthread_create(&table->observer, NULL, observe_experiment, philo);
		join_threads(&philo);
		pthread_join(table->observer, NULL);
	}
	pthread_mutex_destroy(&table->table_lock);
	erase_group(&philo);
}

void initiate_threads(t_philosopher **philosophers)
{
	t_philosopher *current;

	current = *philosophers;
	while (current)
	{
		if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
		{
			perror("Failed to create thread");
			return ;
		}
		current = current->next;
	}
}

void	join_threads(t_philosopher **philosopher)
{
	t_philosopher *current;

	current = *philosopher;
	while (current)
	{
		pthread_join(current->thread, NULL);
		current = current->next;
	}
}

void	erase_group(t_philosopher **group)
{
	t_philosopher *current;
	t_philosopher *temp;

	current = *group;
	while (current)
	{
		temp = current->next;
		pthread_mutex_destroy(&current->right_fork->lock);
		pthread_mutex_destroy(&current->philo_mutex);
		free(current->right_fork);
		free(current);
		current = temp;
	}
	*group = NULL;
}

int get_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
