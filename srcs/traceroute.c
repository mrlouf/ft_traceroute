/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:28:40 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/26 12:08:45 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

static void send_probes(t_traceroute *t)
{
    for (int i = 0; i < t->tries; i++)
    {
        if (g_sigint)
            return;
        printf("* ");
        fflush(stdout);
        // Simulate waiting for a response (in a real implementation, you'd use select/poll)
        usleep(500000); // 0.5 second
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
        send_probes(t);
        t->current_hop++;
        printf("\n");
    }
}