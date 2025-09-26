/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:28:40 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/26 12:48:49 by nponchon         ###   ########.fr       */
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
    ssize_t bytes_sent;

    ft_memset(send_buf, 0, sizeof(send_buf));
    icmp_hdr = (struct icmp *)send_buf;

    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_id = htons(getpid() & 0xFFFF);
    icmp_hdr->icmp_seq = htons(t->seq++);
    ft_memset(icmp_hdr->icmp_data, 0x42, sizeof(icmp_hdr->icmp_data)); // Fill data with arbitrary bytes
    icmp_hdr->icmp_cksum = 0;
    icmp_hdr->icmp_cksum = checksum((unsigned short *)icmp_hdr, sizeof(send_buf));

    bytes_sent = sendto(t->socket, send_buf, sizeof(send_buf), 0,
                        (struct sockaddr *)&t->addr, sizeof(t->addr));
    if (bytes_sent < 0)
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
}

void	receive_packet(t_traceroute *t)
{
	char recv_buf[512];
	struct sockaddr_in recv_addr;
	socklen_t addr_len = sizeof(recv_addr);
	ssize_t bytes_received;
	struct ip *ip_hdr;
	struct icmp *icmp_hdr;
	struct timeval timeout = {5, 0}; // 1 second timeout

	setsockopt(t->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	bytes_received = recvfrom(t->socket, recv_buf, sizeof(recv_buf), 0,
							  (struct sockaddr *)&recv_addr, &addr_len);
	if (bytes_received < 0)
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
		struct ip *orig_ip = (struct ip *)(icmp_hdr->icmp_data);
		struct icmp *orig_icmp = (struct icmp *)(orig_ip + (orig_ip->ip_hl << 2));

		if (orig_icmp->icmp_id == htons(getpid() & 0xFFFF))
		{
			char addr_str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &recv_addr.sin_addr, addr_str, sizeof(addr_str));
			printf(" %s", addr_str);
			fflush(stdout);
		}
	}
	else if (icmp_hdr->icmp_type == ICMP_ECHOREPLY)
	{
		if (icmp_hdr->icmp_id == htons(getpid() & 0xFFFF))
		{
			char addr_str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &recv_addr.sin_addr, addr_str, sizeof(addr_str));
			printf(" %s", addr_str);
			fflush(stdout);
			g_sigint = 1; // Stop traceroute on echo reply
		}
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

		for (int i = 0; i < t->tries; i++)
		{
			if (g_sigint) {
				break;
			}

			// Set the TTL for the socket
			if (setsockopt(t->socket, IPPROTO_IP, IP_TTL, &t->current_hop, sizeof(t->current_hop)) < 0) {
				perror("setsockopt");
				exit(EXIT_FAILURE);
			}

			send_icmp_packet(t);
			receive_packet(t);
		}

        t->current_hop++;
        printf("\n");
    }
}