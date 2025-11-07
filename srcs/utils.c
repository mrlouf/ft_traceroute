/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <nicolas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:27:36 by nponchon          #+#    #+#             */
/*   Updated: 2025/11/05 17:58:46 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

/* 
	Helper function to print hop information with proper formatting without
	using fflush() which is not allowed.
*/
void	print_hopinfo(int hop)
{
	ft_putnbr_fd(hop, STDOUT_FILENO);
	if (hop < 10)
		ft_putstr_fd("  ", STDOUT_FILENO);
	else if (hop < 100)
		ft_putstr_fd(" ", STDOUT_FILENO);
}

void	print_message(t_traceroute *t, char *addr_str)
{
	struct timeval	tv;
	float			time_ms;

	if (t->seq == 0) {
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(addr_str, STDOUT_FILENO);
	}
	if (gettimeofday(&tv, NULL) == -1) {
		error_exit("gettimeofday");
	}
	tv.tv_sec = tv.tv_sec - t->seconds;
	tv.tv_usec = tv.tv_usec - t->microseconds;
	time_ms = tv.tv_sec * 1000 + (float)tv.tv_usec / 1000;

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "  %.3fms", time_ms);
	ft_putstr_fd(buffer, STDOUT_FILENO);
}

void	error_exit(const char *msg)
{
	printf("%s: %s\n", msg, strerror(errno));
	exit(EXIT_FAILURE);
}