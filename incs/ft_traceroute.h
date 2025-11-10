/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:02:45 by nponchon          #+#    #+#             */
/*   Updated: 2025/11/10 14:47:27 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

// Macros
# define EXIT_USAGE 64
# define EXIT_NOROOT 77

# define HELP_MSG "Usage: sudo ./ft_traceroute [OPTION...] HOST\n\
Print the route packets trace to network host.\n\
\n\
  -f, --first-hop=NUM        set initial hop distance, i.e., time-to-live\n\
  -m, --max-hop=NUM          set maximal hop count (default: 64)\n\
  -q, --tries=NUM            send NUM probe packets per hop (default: 3)\n\
      --resolve-hostnames    resolve hostnames\n\
  -w, --wait=NUM             wait NUM seconds for response (default: 3)\n\
      --help                 give this help list\n\
      --usage                give a short usage message\n"

# define USAGE_MSG "Usage: sudo ./ft_traceroute [-f NUM] [-m NUM] [-q NUM] [-w NUM]\n\
            [--first-hop=NUM] [--max-hop=NUM] [--tries=NUM] [--wait=NUM] [--resolve-hostnames]\n\
            [--help] [--usage] HOST\n"

// Headers
# include <stdlib.h>		// For EXIT_SUCCESS / EXIT_FAILURE
# include <stdio.h>			// For printf functions
# include <stdint.h>		// For uint8_t
# include <errno.h>
# include <signal.h>

# include <sys/types.h>		// For network addresses
# include <sys/socket.h>  // For socket functions
# include <netdb.h>		    // For getaddrinfo
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>	// For ICMP headers

# include <sys/time.h>		// For gettimeofday

# include "../libft/libft.h"

// Bonus:
#include <fcntl.h>    // For fcntl (non blocking sockets for wait time 0)

// Structure

typedef struct s_traceroute
{
	char				*target;
	char                *ip;
	struct sockaddr_in	addr;
	char				*fqdn;
	
	int					socket;
	int					ident;
	int					seq;
	uint8_t				current_hop;
	uint8_t				max_hop;
	uint8_t				tries;
	uint8_t				timeout;
	uint8_t				dnsresolve;
	uint16_t			packet_size;

	uint64_t			seconds;
	uint64_t			microseconds;
}	t_traceroute;

// Global variable
extern int g_sigint;

// Prototypes
void	parse_args(char **arg, t_traceroute *t);
void	initialise_data(t_traceroute *t);
void	resolve_hostname(t_traceroute *t);
void	resolve_fqdn(t_traceroute *t);
void	open_socket(t_traceroute *t);
void	start_traceroute(t_traceroute *t);
void	print_hopinfo(int hop);
void	print_message(t_traceroute *t, char *addr_str);
void	error_exit(const char *msg);

#endif
