/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:28:40 by nponchon          #+#    #+#             */
/*   Updated: 2025/11/08 14:17:00 by nponchon         ###   ########.fr       */
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
    struct icmp     *icmp_hdr;
    char            send_buf[t->packet_size];
	struct timeval  tv;

	if (gettimeofday(&tv, NULL) == -1) {
		error_exit("gettimeofday");
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
        error_exit("sendto");
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
			ft_putstr_fd(" * ", STDOUT_FILENO);
			return;
		}
		error_exit("recvfrom");
	}

	ip_hdr = (struct ip *)recv_buf;
	icmp_hdr = (struct icmp *)(recv_buf + (ip_hdr->ip_hl << 2));

    if (icmp_hdr->icmp_type == ICMP_TIME_EXCEEDED)
    {
        char addr_str[INET_ADDRSTRLEN];
        char *tmp = inet_ntoa(recv_addr.sin_addr);
        if (tmp == NULL)
            error_exit("inet_ntoa");
        strncpy(addr_str, tmp, INET_ADDRSTRLEN);
        addr_str[INET_ADDRSTRLEN - 1] = '\0';
        print_message(t, addr_str);
    }
    else if (icmp_hdr->icmp_type == ICMP_ECHOREPLY)
    {
        char addr_str[INET_ADDRSTRLEN];
        char *tmp = inet_ntoa(recv_addr.sin_addr);
        if (tmp == NULL)
            error_exit("inet_ntoa");
        strncpy(addr_str, tmp, INET_ADDRSTRLEN);
        addr_str[INET_ADDRSTRLEN - 1] = '\0';
        print_message(t, addr_str);

        // Final destination reached
        if (t->seq + 1 == t->tries)
            g_sigint = 1;
    }
    // If we reach here, packet is not for us
    else
        ft_putstr_fd(" * ", STDOUT_FILENO);
}

void    start_traceroute(t_traceroute *t)
{
    uint8_t ttl = 0;

    printf("traceroute to %s (%s), %d hops max\n",
           t->target, t->ip, t->max_hop);

    while (t->current_hop <= t->max_hop && !g_sigint)
    {
        print_hopinfo(++ttl);

		for (t->seq = 0; t->seq < t->tries; t->seq++)
		{
			if (g_sigint) {
				break;
			}
			// Update the TTL for the socket
			if (setsockopt(t->socket, IPPROTO_IP, IP_TTL, &t->current_hop, sizeof(t->current_hop)) < 0) {
				error_exit("setsockopt");
			}
			send_icmp_packet(t);
			receive_packet(t);
		}

        ft_putstr_fd("\n", STDOUT_FILENO);
        t->current_hop++;
    }
    free(t->fqdn);
    close(t->socket);
}