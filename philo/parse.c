/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:46:12 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/21 19:52:25 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

bool    valid_arguments(char **arguments)
{
    int i;
    int j;
    int len;
    
    if (!arguments)
        return (0);
    j = 0;
    while (arguments[j])
    {
        i = 0;
        len = 0;
        if (!arguments[j] || !arguments[j][0])
            return (0);
        while (arguments[j][i] == '0')
            i++;
        while (arguments[j][i++])
            len++;
        if (len >= 11)
            return (0);
        arguments++;
    }
    return (1);
}

t_table *parse_table(char **arguments)
{
    t_table *table;

    if (!valid_arguments(arguments))
        return (NULL);
    table = malloc(sizeof(t_table));
    if (!table)
        return (NULL);
    table->number_of_times_to_eat = -1;
    if (arguments[5])
        table->number_of_times_to_eat = ft_atol(arguments[5]);
    table->philosopher_num = ft_atol(arguments[1]);
	table->time_to_die = ft_atol(arguments[2]);
	table->time_to_eat = ft_atol(arguments[3]);
	table->time_to_sleep = ft_atol(arguments[4]);
	table->start_time = get_time();
	table->threads_initiated = 0;
	table->end_experiment = 0;
	table->all_threads_ready = 0;
    if (table->philosopher_num > INT_MAX || table->time_to_die > INT_MAX
        || table->time_to_eat > INT_MAX || table->time_to_sleep > INT_MAX
        || table->number_of_times_to_eat > INT_MAX || table->philosopher_num < 0
        || table->time_to_die < 0 || table->time_to_eat < 0
        || table->time_to_sleep < 0)
            return (NULL);
    return (table);
}
