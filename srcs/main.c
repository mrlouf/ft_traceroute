/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:03:01 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/25 16:36:49 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

static void    usage_error(void)
{
    fprintf(stderr, "ft_traceroute: missing host operand\n\
Try 'ft_traceroute --help' or 'ft_traceroute --usage' for more information.\n");

    exit(EXIT_USAGE);
}

static void    root_error(void)
{
    fprintf(stderr, "ft_traceroute: must be run as root\n");
    exit(EXIT_NOROOT);
}

int main(int ac, char **av)
{
    if (ac != 2) {
        usage_error();
    }
    if (getuid() != 0) {
        root_error();
    }

    t_traceroute t;

    av++;
    parse_arg(*av);
    initialise_data(*av, &t);
    open_socket(&t);
    start_traceroute(&t);

    exit(EXIT_SUCCESS);
}
