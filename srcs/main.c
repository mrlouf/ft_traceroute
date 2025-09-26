/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:03:01 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/26 11:55:26 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

// Global variable
int g_sigint = 0;

static void	usage_error(void)
{
    fprintf(stderr, "ft_traceroute: missing host operand\n\
Try 'ft_traceroute --help' or 'ft_traceroute --usage' for more information.\n");

    exit(EXIT_USAGE);
}

static void	root_error(void)
{
    fprintf(stderr, "ft_traceroute: must be run as root\n");
    exit(EXIT_NOROOT);
}

void		sigint_handler(int sig) {
	(void)sig;
	g_sigint = 1;
}

int main(int ac, char **av)
{
    if (ac != 2) {
        usage_error();
    }
    if (getuid() != 0) {
        root_error();
    }

	struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    t_traceroute t;
    av++;
    parse_arg(*av);
    initialise_data(*av, &t);
    open_socket(&t);
    start_traceroute(&t);

    exit(EXIT_SUCCESS);
}
