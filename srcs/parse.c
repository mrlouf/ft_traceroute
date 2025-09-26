/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:39:47 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/26 12:04:54 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void    resolve_fqdn(t_traceroute *t)
{
	char host[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&t->addr, sizeof(t->addr),
					host, sizeof(host), NULL, 0, 0) == 0) {
		t->fqdn = strdup(host);
	} else {
		fprintf(stderr, "ft_traceroute: Could not get full hostname for IP \"%s\"\n", t->ip);
		exit(EXIT_FAILURE);
	}
}

void    resolve_hostname(t_traceroute *t)
{
   	struct addrinfo hints = {0}, *res;
	hints.ai_family = AF_INET; // IPv4 only; AF_UNSPEC is for IPv4/6

	if (getaddrinfo(t->target, NULL, &hints, &res) == 0) {
		struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
		static char ipstr[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &(addr->sin_addr), ipstr, sizeof(ipstr));
		t->ip = ipstr;
		t->addr = *addr;

		freeaddrinfo(res);
	} else {
		fprintf(stderr, "ft_traceroute: unknown host\n");
		exit(EXIT_FAILURE);
	}
}

void    initialise_data(char *av, t_traceroute *t)
{
    t->target = av;
    t->ip = NULL;
	t->fqdn = NULL;

	t->socket = -1;
	t->ident = -1;
	t->current_hop = 1;
	t->max_hop = 64;		// Default max hops
	t->packet_size = 60;	// Default packet size
	t->tries = 3;			// Default number of tries per hop

    resolve_hostname(t);
    resolve_fqdn(t);

}

void    parse_arg(char *arg)
{
    if (ft_strcmp(arg, "--help") == 0) {
        fprintf(stdout, HELP_MSG);
        exit(EXIT_SUCCESS);
    } else if (ft_strcmp(arg, "--usage") == 0) {
        fprintf(stdout, USAGE_MSG);
        exit(EXIT_SUCCESS);
    }

    // TODO: parse options for the bonus
    // parse_options();

}
