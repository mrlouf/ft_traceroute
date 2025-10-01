/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:28:40 by nponchon          #+#    #+#             */
/*   Updated: 2025/10/01 16:30:29 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

unsigned short    checksum(unsigned short *buf, int len)
{
    unsigned long sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

static void send_icmp_packet(t_traceroute *t)
{
    struct icmp *icmp_hdr;
    char send_buf[t->packet_size];
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1) {
		perror("gettimeofday");
		exit(EXIT_FAILURE);
	}
	t->seconds = tv.tv_sec;
	t->microseconds = tv.tv_usec;

	ft_memset(send_buf, 0, sizeof(send_buf));
	icmp_hdr = (struct icmp *)send_buf;

    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_id = htons(getpid() & 0xFFFF);
    icmp_hdr->icmp_seq = htons(t->seq);
    ft_memset(icmp_hdr->icmp_data, 0x42, sizeof(icmp_hdr->icmp_data)); // Fill data with arbitrary bytes
    icmp_hdr->icmp_cksum = 0;
    icmp_hdr->icmp_cksum = checksum((unsigned short *)icmp_hdr, sizeof(send_buf));

    if (sendto(t->socket, send_buf, sizeof(send_buf), 0,
                        (struct sockaddr *)&t->addr, sizeof(t->addr)) < 0)
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
}

void	receive_packet(t_traceroute *t)
{
	char 				recv_buf[512];
	struct sockaddr_in	recv_addr;
	socklen_t 			addr_len = sizeof(recv_addr);
	struct ip			*ip_hdr;
	struct icmp			*icmp_hdr;

	if (recvfrom(t->socket, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&recv_addr, &addr_len) < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			printf(" *");
			return;
		}
		perror("recvfrom");
		exit(EXIT_FAILURE);
	}

	ip_hdr = (struct ip *)recv_buf;
	icmp_hdr = (struct icmp *)(recv_buf + (ip_hdr->ip_hl << 2));

	if (icmp_hdr->icmp_type == ICMP_TIME_EXCEEDED)
	{
		char addr_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &recv_addr.sin_addr, addr_str, sizeof(addr_str));
		print_message(t, addr_str);
	}
	else if (icmp_hdr->icmp_type == ICMP_ECHOREPLY)
	{
		char addr_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &recv_addr.sin_addr, addr_str, sizeof(addr_str));
		print_message(t, addr_str);

		// Final destination reached
		if (t->seq + 1 == t->tries)
			g_sigint = 1;
	}
}

void    start_traceroute(t_traceroute *t)
{
    printf("traceroute to %s (%s), %d hops max\n",
           t->target, t->ip, t->max_hop);

    while (t->current_hop <= t->max_hop && !g_sigint)
    {
        printf("%2d  ", t->current_hop);
        fflush(stdout);

		for (t->seq = 0; t->seq < t->tries; t->seq++)
		{
			if (g_sigint) {
				break;
			}
			// Update the TTL for the socket
			if (setsockopt(t->socket, IPPROTO_IP, IP_TTL, &t->current_hop, sizeof(t->current_hop)) < 0) {
				perror("setsockopt");
				exit(EXIT_FAILURE);
			}
			send_icmp_packet(t);
			receive_packet(t);
		}

        t->current_hop++;
    }
}