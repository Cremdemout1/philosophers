/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/03 15:10:44 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int    get_int(pthread_mutex_t *mutex, int *value)
{
    int new;
    
    pthread_mutex_lock(mutex);
    new = *value;
    pthread_mutex_unlock(mutex);
    return (new);
}

void    set_int(pthread_mutex_t *mutex, int *var, int value)
{
    pthread_mutex_lock(mutex);
    *var = value;
    pthread_mutex_unlock(mutex);
}

int    get_long(pthread_mutex_t *mutex, long *value)
{
    long new;
    
    pthread_mutex_lock(mutex);
    new = *value;
    pthread_mutex_unlock(mutex);
    return (new);
}

void    set_long(pthread_mutex_t *mutex, long *var, long value)
{
    pthread_mutex_lock(mutex);
    *var = value;
    pthread_mutex_unlock(mutex);
}

int    sim_end(t_table *table)
{
    return (get_int(&table->table_lock, &table->end_experiment));  
}

int is_dead(t_philosopher *philo)
{
    long time_passed;
    long time_to_die;

    if (get_int(&philo->philo_mutex, &philo->full))
        return (0);
    time_passed = get_time() - get_long(&philo->philo_mutex, &philo->last_meal_time);
    time_to_die = philo->table->time_to_die;
    if (time_passed >= time_to_die)
        return (1);
    return (0);
}


void _usleep(unsigned long long time, t_philosopher *philo)
{
    unsigned long start;

    start = get_time();
    while (get_time() - start < time)
    {
        
        if (is_dead(philo)) // Check if the philosopher is dead
            return; // Exit the function if the philosopher is dead
        usleep(10);
    }
}


int check_if_full(t_philosopher *philo)
{
    int required_meals;

    required_meals = philo->table->number_of_times_to_eat;
    if (required_meals >= 0 && philo->times_eaten == required_meals)
    {
        printf("\033[1;32m%ld %ld is full\033[0m\n", get_time() - philo->table->start_time, philo->id);
        return (1);
    }
    return (0);
}

// void all_threads_ready(t_philosopher *philo)
// {
//     while (get_int(&philo->table->table_lock, &philo->table->all_threads_ready))
//         ;
// }

void all_threads_ready(t_philosopher *philo)
{
    while (1)
    {
        pthread_mutex_lock(&philo->table->table_lock);
        if (philo->table->all_threads_ready)
        {
            pthread_mutex_unlock(&philo->table->table_lock);
            break;
        }
        else
            pthread_mutex_unlock(&philo->table->table_lock);
    }
}

void    write_status(t_status status, t_philosopher *philo)
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

void initiate_threads(t_philosopher **philosophers)
{
    t_philosopher *current;

    current = *philosophers;
    while (current)
    {
        if (current->id % 2 == 0)
        {
            if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
            {
                perror("Failed to create thread");
                return ;
            }
        }
        current = current->next;
    }
    usleep(50);
    current = *philosophers;
    while (current)
    {
        if (current->id % 2 == 1)
        {
            if (pthread_create(&current->thread, NULL, &philosopher_routine, current) != 0)
            {
                perror("Failed to create thread");
                return ;
            }
        }
        current = current->next;
    }
}

void    join_threads(t_philosopher **philosopher)
{
    t_philosopher *current;

    current = *philosopher;
    while (current)
    {
        pthread_join(current->thread, NULL);
        current = current->next;
    }
}
void *observe_experiment(void *phil)
{
    t_philosopher *philo;
    t_table *table;
    t_philosopher *current;
    int all_full;
    
    philo = (t_philosopher *)phil;
    table = philo->table;
    while (1)
    {
        long initiated = get_long(&table->table_lock, &table->threads_initiated);
        if (initiated == table->philosopher_num)
            break;
        usleep(50);
    }
    while (!sim_end(table))
    {
        current = table->philo_list;
        all_full = 1;
        while (current)
        {
            //printf("Philosopher %ld is %s\n", current->id, get_int(&current->philo_mutex, &current->full) ? "full" : "not full");
            if (is_dead(current))
            {
                set_int(&table->table_lock, &table->end_experiment, 1);
                write_status(DEAD, current);
                return NULL;
            }
            if (!get_int(&current->philo_mutex, &current->full)) // Check if any is not full
                all_full = 0;
            current = current->next;
        }
        if (all_full)
        {
            printf("\033[1;30m%ld everyone is full\033[0m\n", get_time() - philo->table->start_time);
            set_int(&table->table_lock, &table->end_experiment, 1);
            return NULL;
        }
        usleep(50);
    }
    return (NULL);
}
