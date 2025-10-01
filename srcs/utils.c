/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:27:36 by nponchon          #+#    #+#             */
/*   Updated: 2025/10/01 16:40:47 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void	print_message(t_traceroute *t, char *addr_str)
{
	struct timeval	tv;
	float			time_ms;

	if (t->seq == 0) {
		printf(" %s", addr_str);
	}
	if (gettimeofday(&tv, NULL) == -1) {
		error_exit("gettimeofday");
	}
	tv.tv_sec = tv.tv_sec - t->seconds;
	tv.tv_usec = tv.tv_usec - t->microseconds;
	time_ms = tv.tv_sec * 1000 + (float)tv.tv_usec / 1000;
	printf("  %.3fms", time_ms);

	if (t->seq + 1 == t->tries) {
		printf("\n");
	}
}

void	error_exit(const char *msg)
{
	printf("%s: %s\n", msg, strerror(errno));
	exit(EXIT_FAILURE);
}