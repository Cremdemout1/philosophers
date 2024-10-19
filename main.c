/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 11:49:50 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/19 16:00:17 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	erase_group(t_philosopher **group)
{
	t_philosopher *current;
	t_philosopher *temp;

	current = *group;
	while (current)
	{
		temp = current->next;
		pthread_mutex_destroy(&current->right_fork->lock); // is it necessary???
		pthread_mutex_destroy(&current->philo_mutex);
		free(current->right_fork);
		free(current);
		current = temp;
	}
	*group = NULL;
}


void init_experiment(t_table *table)
{
	t_philosopher *philo;

	if (pthread_mutex_init(&table->table_lock, NULL) != 0)
	{
		perror("Failed to initialize table mutex");
		exit(1);
	}
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
	{
		perror("Failed to initialize write mutex");
		exit(1);
	}
	philo = NULL;
	if (create_philosophers(table, &philo) == 0)
	{
		if (!philo)
		{
			printf("Failed to create philo\n");
			exit(1);
		}
		table->philo_list = philo;
		initiate_threads(&philo);
		pthread_create(&table->observer, NULL, observe_experiment, philo);
		join_threads(&philo);
		pthread_join(table->observer, NULL);
	}
	pthread_mutex_destroy(&table->table_lock);
	erase_group(&philo);
}

int main(int argc, char **argv)
{
	t_table *table;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Argument count invalid\n");
		return (1);
	}
	table = malloc(sizeof(t_table));
	if (!table)
		return (perror("Failed to allocate memory for table"), 1);
	table->philosopher_num = atoi(argv[1]);
	table->time_to_die = atoi(argv[2]);
	table->time_to_eat = atoi(argv[3]);
	table->time_to_sleep = atoi(argv[4]);
	table->start_time = get_time();
	table->threads_initiated = 0;
	table->end_experiment = 0;
	table->all_threads_ready = 0;

	if (table->time_to_die < 60 || table->time_to_eat < 60 || table->time_to_sleep < 60)
	{
		free(table);
		return (printf("Invalid given time. Please give a value over 60 milliseconds\n"), 1);
	}
	if (argc == 6)
	{
		table->number_of_times_to_eat = atoi(argv[5]);
		if (table->number_of_times_to_eat < 0)
		{
			free(table);
			return (printf("Number of meals must be omitted or above 0\n"), 1);
		}
	}
	else
		table->number_of_times_to_eat = -1;
	init_experiment(table);
	free(table);
	return (0);
}
