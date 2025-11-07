/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <nicolas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:13:06 by nponchon          #+#    #+#             */
/*   Updated: 2025/11/07 16:41:37 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void    open_socket(t_traceroute *t)
{
    if ((t->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "Socket creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    t->ident = getpid() & 0xFFFF;

    t->addr.sin_family = AF_INET;
    t->addr.sin_port = 0; // Ports are not used in ICMP

    if (inet_pton(AF_INET, t->ip, &t->addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", t->ip);
        exit(EXIT_FAILURE);
    }

    if (setsockopt(t->socket, IPPROTO_IP, IP_TTL,
                   &t->current_hop, sizeof(t->current_hop)) != 0) {
        fprintf(stderr, "Error setting socket options: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct timeval timeout;
    timeout.tv_sec = t->timeout;
    timeout.tv_usec = 0;
    if (t->timeout == 0) {
        fcntl(t->socket, F_SETFL, O_NONBLOCK);
    } else if (setsockopt(t->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0) {
        fprintf(stderr, "Error setting socket options: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
