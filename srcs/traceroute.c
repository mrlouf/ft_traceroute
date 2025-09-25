/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:28:40 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/25 16:52:59 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void    start_traceroute(t_traceroute *t)
{
    while (t->current_hop <= t->max_hop && !g_sigint)
    {
        printf("%2d  ", t->current_hop);
        fflush(stdout);
        send_probes(t);
        t->current_hop++;
        printf("\n");
    }
}