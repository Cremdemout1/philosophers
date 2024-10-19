/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/19 14:27:38 by ycantin          ###   ########.fr       */
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
            pthread_mutex_t lock;
}       t_fork;

typedef struct s_philosopher t_philosopher;

typedef struct s_table
{
    long                    start_time;
    long                    philosopher_num;
    long                    time_to_die;
    long                    time_to_eat;
    long                    time_to_sleep;
    long                    number_of_times_to_eat;
    long                    threads_initiated;
    int                     all_threads_ready;
    int                     end_experiment;
    pthread_t               observer;
    t_philosopher           *philo_list;
    pthread_mutex_t         table_lock;
    pthread_mutex_t         write_lock;
}   t_table;

typedef struct s_philosopher
{
    long                    id;
    long                    last_meal_time;
    int                     full;
    long                    times_eaten;
    t_fork                  *left_fork;
    t_fork                  *right_fork;
    t_table                 *table;
    pthread_t               thread;
    pthread_mutex_t         philo_mutex;
    struct s_philosopher    *next;
}   t_philosopher;

void    init_experiment(t_table *table);
int     create_philosophers(t_table *table, t_philosopher **group);
void    initiate_threads(t_philosopher **philosophers);
void    *observe_experiment(void *phil);
void    join_threads(t_philosopher **philosopher);
void    erase_group(t_philosopher **group);

int     get_time(void);int check_if_full(t_philosopher *philo);
int     is_dead(t_philosopher *philo);
int     sim_end(t_table *table);
void    _usleep(unsigned long long time, t_philosopher *philo);
void    eat(t_philosopher *philo);

void    write_status(t_status status, t_philosopher *philo);
void    set_long(pthread_mutex_t *mutex, long *var, long value);
int     get_long(pthread_mutex_t *mutex, long *value);
void    set_int(pthread_mutex_t *mutex, int *var, int value);
int     get_int(pthread_mutex_t *mutex, int *value);

#endif



