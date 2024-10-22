/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:58:54 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/22 14:41:16 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

int main(int argc, char **argv)
{
    t_table *table;
    if (argc != 5 && argc != 6)
        return (write(2, "Parser->Error: invalid input\n", 29), 1);
    table = parse_table(argv);
    if (!table)
        return (write(2, "Parser->Error: invalid input\n", 29), 1);
    init_experiment(table);
    free(table);
    return (0);
}