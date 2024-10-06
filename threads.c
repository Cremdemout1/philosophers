/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 11:49:50 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/05 12:43:21 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void *philosopher_routine(void *arg)
{
	t_philosopher *philo;
		
	philo = (t_philosopher *)arg;
		
	//all_threads_ready(philo);
	//set_long(&philo->table->table_lock, &philo->table->start_time, get_time());
		
	//set last meal time
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time());
	//increase threads initiated
	pthread_mutex_lock(&philo->table->table_lock);
	philo->table->threads_initiated++;
	pthread_mutex_unlock(&philo->table->table_lock);
	while (!sim_end(philo->table))
	{
		if(get_int(&philo->philo_mutex, &philo->full))  //for optimization purposes
		{
			printf("\033[1;32m%ld %ld is full\033[0m\n", get_time() - philo->table->start_time, philo->id);
			break ;
		}
		eat(philo);
		if(get_int(&philo->philo_mutex, &philo->full))
		{
			printf("\033[1;32m%ld %ld is full\033[0m\n", get_time() - philo->table->start_time, philo->id);
			break ;
		}
		write_status(SLEEPING, philo);
		_usleep(philo->table->time_to_sleep, philo);
		write_status(THINKING, philo);
	}
	return (NULL);
}

// void init_even(t_philosopher *current)
// {
//   while (current)
//   {
//	 if (current->id % 2 == 0)
//	 {
//	   if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
//	   {
//		 perror("Failed to create thread");
//		 return ;
//	   }
//	 }
//	 current = current->next;
//   }
// }

// void init_odd(t_philosopher *current)
// {
//   while (current)
//   {
//	 if (current->id % 2 == 1)
//	 {
//	   if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
//	   {
//		 perror("Failed to create thread");
//		 return ;
//	   }
//	 }
//	 current = current->next;
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
        printf("\033[1;30m%ld everyone is full\033[0m\n", get_time() - table->start_time);
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
        usleep(50);
    }
    while (!sim_end(table))
    {
		if (observation_round(table))
			break ;
        usleep(50);
    }
    return (NULL);
}
