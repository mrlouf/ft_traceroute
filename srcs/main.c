/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:03:01 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/23 18:10:31 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

static void    usage_error(void)
{
    printf("ft_traceroute: missing host operand\n\
Try 'ft_traceroute --help' or 'ft_traceroute --usage' for more information.\n");

    exit(EXIT_USAGE);
}

int main(int ac, char **av)
{
    if (ac != 2) {
        usage_error();
    }

    av++;
    parse_arg(*av);
/*     initialise_data(av);
    start_traceroute(); */

    exit(EXIT_SUCCESS);
}
