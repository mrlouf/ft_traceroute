/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:02:45 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/25 16:48:28 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

// Macros
# define EXIT_USAGE 64
# define EXIT_NOROOT 77

# define HELP_MSG "Usage: ft_traceroute [OPTION...] HOST\n\
Print the route packets trace to network host.\n\
\n\
  -f, --first-hop=NUM        set initial hop distance, i.e., time-to-live\n\
  -g, --gateways=GATES       list of gateways for loose source routing\n\
  -I, --icmp                 use ICMP ECHO as probe\n\
  -m, --max-hop=NUM          set maximal hop count (default: 64)\n\
  -M, --type=METHOD          use METHOD (`icmp' or `udp') for traceroute\n\
                             operations, defaulting to `udp'\n\
  -p, --port=PORT            use destination PORT port (default: 33434)\n\
  -q, --tries=NUM            send NUM probe packets per hop (default: 3)\n\
      --resolve-hostnames    resolve hostnames\n\
  -t, --tos=NUM              set type of service (TOS) to NUM\n\
  -w, --wait=NUM             wait NUM seconds for response (default: 3)\n\
  -?, --help                 give this help list\n\
      --usage                give a short usage message\n\
  -V, --version              print program version\n"

# define USAGE_MSG "Usage: ft_traceroute [-I?V] [-f NUM] [-g GATES] [-m NUM] [-M METHOD] [-p PORT]\n\
            [-q NUM] [-t NUM] [-w NUM] [--first-hop=NUM] [--gateways=GATES]\n\
            [--icmp] [--max-hop=NUM] [--type=METHOD] [--port=PORT]\n\
            [--tries=NUM] [--resolve-hostnames] [--tos=NUM] [--wait=NUM]\n\
            [--help] [--usage] [--version] HOST\n"

// Headers
# include <stdlib.h>		// For EXIT_SUCCESS / EXIT_FAILURE
# include <stdio.h>			// For printf functions
# include <stdint.h>		// For uint8_t
# include <errno.h>
# include <signal.h>

# include <sys/types.h>		// For network addresses
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>

# include "../libft/libft.h"

// Structure

typedef struct s_traceroute
{
	char				*target;
	char                *ip;
	struct sockaddr_in	addr;
	char				*fqdn;
	
	int					socket;
	int					ident;
	uint8_t				ttl;
}	t_traceroute;

// Prototypes
void	parse_arg(char *arg);
void	initialise_data(char *arg, t_traceroute *t);
void 	resolve_hostname(t_traceroute *t);
void	resolve_fqdn(t_traceroute *t);
void    open_socket(t_traceroute *t);
void    start_traceroute(t_traceroute *t);

#endif
