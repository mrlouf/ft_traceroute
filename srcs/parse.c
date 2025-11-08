/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:39:47 by nponchon          #+#    #+#             */
/*   Updated: 2025/11/08 14:15:08 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void    resolve_fqdn(t_traceroute *t)
{
	char host[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&t->addr, sizeof(t->addr),
					host, sizeof(host), NULL, 0, 0) == 0) {
		t->fqdn = ft_strdup(host);
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

void    initialise_data(t_traceroute *t)
{
    t->target = NULL;
    t->ip = NULL;
	t->fqdn = NULL;

	t->socket = -1;
	t->ident = -1;
	t->seq = 0;
	t->current_hop = 1;		// Default starting hop
	t->max_hop = 64;		// Default max hops
	t->packet_size = 60;	// Default packet size
	t->tries = 3;			// Default number of tries per hop
	t->timeout = 3;			// Default timeout in seconds
	t->dnsresolve = 0;		// By default, do not resolve hostnames

    t->seconds = 0;
    t->microseconds = 0;


}

void    parse_args(char **arg, t_traceroute *t)
{
	while (*arg)
	{
		if (ft_strcmp(*arg, "--help") == 0) {
			printf(HELP_MSG);
			exit(EXIT_SUCCESS);
		} else if (ft_strcmp(*arg, "--usage") == 0) {
			printf(USAGE_MSG);
			exit(EXIT_SUCCESS);
		} else if (ft_strcmp(*arg, "-q") == 0 || ft_strcmp(*arg, "--tries") == 0) {
			arg++;
			if (*arg) {
				t->tries = ft_atoi(*arg);
				if (t->tries < 1 || t->tries > 10) {
					fprintf(stderr, "ft_traceroute: tries must be between 1 and 10\n");
					exit(EXIT_FAILURE);
				}
			} else {
				fprintf(stderr, "ft_traceroute: option '%s' requires an argument\n", *(arg - 1));
				exit(EXIT_FAILURE);
			}
		} else if (ft_strcmp(*arg, "-m") == 0 || ft_strcmp(*arg, "--max-hop") == 0) {
			arg++;
			if (*arg) {
				t->max_hop = ft_atoi(*arg);
				if (t->max_hop < 1) {
					fprintf(stderr, "ft_traceroute: max hops must be between 1 and 255\n");
					exit(EXIT_FAILURE);
				}
			} else {
				fprintf(stderr, "ft_traceroute: option '%s' requires an argument\n", *(arg - 1));
				exit(EXIT_FAILURE);
			}
		} else if (ft_strcmp(*arg, "-w") == 0 || ft_strcmp(*arg, "--wait") == 0) {
			arg++;
			if (*arg) {
				t->timeout = ft_atoi(*arg);
				if (t->timeout > 10) {
					fprintf(stderr, "ft_traceroute: wait time must be between 1 and 10 seconds\n");
					exit(EXIT_FAILURE);
				}
			} else {
				fprintf(stderr, "ft_traceroute: option '%s' requires an argument\n", *(arg - 1));
				exit(EXIT_FAILURE);
			}
		} else if (ft_strcmp(*arg, "-f") == 0 || ft_strcmp(*arg, "--first-hop") == 0) {
			arg++;
			if (*arg) {
				t->current_hop = ft_atoi(*arg);
				if (t->current_hop < 1) {
					fprintf(stderr, "ft_traceroute: first hop must be between 1 and 255\n");
					exit(EXIT_FAILURE);
				}
			} else {
				fprintf(stderr, "ft_traceroute: option '%s' requires an argument\n", *(arg - 1));
				exit(EXIT_FAILURE);
			}
		} else if (ft_strcmp(*arg, "--resolve-hostnames") == 0) {
			t->dnsresolve = 1;
			arg++;
			continue;
		} else {
			// Assume the first non-option argument is the target
			if (!t->target)
				t->target = *arg;
			else {
				fprintf(stderr, "ft_traceroute: unexpected argument '%s'\n", *arg);
				exit(EXIT_FAILURE);
			}
		}
		arg++;
	}
	if (!t->target) {
		fprintf(stderr, "ft_traceroute: missing host operand\n");
		exit(EXIT_FAILURE);
	}

    resolve_hostname(t);
    resolve_fqdn(t);
}
