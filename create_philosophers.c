/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philosophers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/09/26 12:42:47 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philosopher *initialize_philosopher(t_table *table, int i)
{
    t_philosopher *philosopher;
    
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
    philosopher->right_fork = malloc(sizeof(t_fork));
    if (!philosopher->right_fork)
    {
        perror("Right fork malloc failed");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&philosopher->right_fork->lock, NULL) != 0)
    {
        free(philosopher->right_fork);
        perror("Mutex init for right fork failed");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_init(&philosopher->philo_mutex, NULL);
    philosopher->left_fork = NULL;
    philosopher->next = NULL;
    return (philosopher);
}

t_philosopher *find_latest_member(t_philosopher *current)
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

void add_to_group(t_philosopher **group, t_philosopher *new_member)
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

void    connect_forks(t_philosopher **group)
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

void create_philosophers(t_table *table, t_philosopher **group)
{
    int i;
    t_philosopher *philosopher;

    i = 0;
    while (i < table->philosopher_num)
    {
        philosopher = initialize_philosopher(table, i);
        printf("Philosopher %ld created\n", philosopher->id);
        add_to_group(group, philosopher);
        i++;
    }
    connect_forks(group);
}

int get_time(void)
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
