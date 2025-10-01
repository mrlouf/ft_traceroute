/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:27:36 by nponchon          #+#    #+#             */
/*   Updated: 2025/10/01 15:46:25 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void	print_intermediate(t_traceroute *t, char *addr_str)
{
	if (t->seq == 0) {
		printf(" %s", addr_str);
	}
	
	printf("  0.0ms");

	if (t->seq + 1 == t->tries) {
		printf("\n");
	}
}
