/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/09/27 13:11:24 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>

typedef enum s_status
{
    FORK1,
    FORK2,
    EATING,
    SLEEPING,
    DEAD,
    THINKING
}t_status;

typedef struct s_fork
{
                    pthread_mutex_t lock;   // Mutex for the fork
}       t_fork;

typedef struct s_philosopher t_philosopher;

typedef struct s_table
{
    long             start_time;
    long             philosopher_num;
    long             time_to_die;
    long             time_to_eat;
    long             time_to_sleep;
    long             number_of_times_to_eat;
    long             threads_initiated;
    int              all_threads_ready;
    int              end_experiment;
    pthread_t        observer;
    t_philosopher    *philo_list;
    pthread_mutex_t  table_lock;
    pthread_mutex_t  write_lock;
}   t_table;

typedef struct s_philosopher
{
    long             id;                // Philosopher's ID
    long            last_meal_time;    // Timestamp of the last meal
    int             full;
    long            times_eaten;       // How many times the philosopher has eaten
    t_fork          *left_fork;        // Pointer to the left fork
    t_fork          *right_fork;       // Pointer to the right fork
    t_table         *table;            // Reference to shared table data
    pthread_t       thread;            // Philosopher's thread
    pthread_mutex_t philo_mutex;
    struct s_philosopher *next;        // Pointer to the next philosopher (circular linked list)
}   t_philosopher;

void    init_experiment(t_table *table);
void    create_philosophers(t_table *table, t_philosopher **group);
void    initiate_threads(t_philosopher **philosophers);
void    *observe_experiment(void *phil);
void    join_threads(t_philosopher **philosopher);
void    erase_group(t_philosopher **group);
int get_time(void);



#endif



