/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 11:49:50 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 16:41:58 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philo;
    
    philo = (t_philosopher *)arg;

    pthread_mutex_lock(&philo->table->table_lock);
    philo->table->threads_initiated++;
    pthread_mutex_unlock(&philo->table->table_lock);
    
    if (philo->id % 2 == 0)
        usleep(10000);
    while (!sim_end(philo->table))
    {
        if (get_int(&philo->philo_mutex, &philo->full))  // For optimization purposes
            break ;
        eat(philo); // Philosopher tries to eat
        if (get_int(&philo->philo_mutex, &philo->full))
            break ;
        write_status(SLEEPING, philo);
        _usleep(philo->table->time_to_sleep, philo);
        write_status(THINKING, philo);
        _usleep(philo->thinking_time, philo);
    }
    return (NULL);
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

// void init_even(t_philosopher *current)
// {
//   while (current)
//   {
// 	 if (current->id % 2 == 0)
// 	 {
// 	   if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
// 	   {
// 		 perror("Failed to create thread");
// 		 return ;
// 	   }
// 	 }
// 	 current = current->next;
//   }
// }

// void init_odd(t_philosopher *current)
// {
//   while (current)
//   {
// 	 if (current->id % 2 == 1)
// 	 {
// 	   if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
// 	   {
// 		 perror("Failed to create thread");
// 		 return ;
// 	   }
// 	 }
// 	 current = current->next;
//   }
// }

// void initiate_threads(t_philosopher **philosophers)
// {
//   t_philosopher *current;
//   current = *philosophers;
//   init_even(current);
//   usleep(50);
//   init_odd(current);
// }

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

int observation_round(t_table *table)
{
    t_philosopher *current;
    int all_full;

	all_full = 1;
    current = table->philo_list;
    while (current)
    {
        if (is_dead(current))
        {
            set_int(&table->table_lock, &table->end_experiment, 1);
            write_status(DEAD, current);
            return (-1);
        }
        if (!get_int(&current->philo_mutex, &current->full))
            all_full = 0;
        current = current->next;
    }
    if (all_full)
    {
        set_int(&table->table_lock, &table->end_experiment, 1);
        return (1);
    }
    return (0);
}

void *observe_experiment(void *phil)
{
    t_philosopher *philo;
    t_table *table;
    long initiated;

    philo = (t_philosopher *)phil;
    table = philo->table;
    while (1)
    {
        initiated = get_long(&table->table_lock, &table->threads_initiated);
        if (initiated == table->philosopher_num)
            break;
        usleep(100);
    }
    while (!sim_end(table))
    {
		if (observation_round(table))
			break ;
        usleep(50);
    }
    return (NULL);
}
