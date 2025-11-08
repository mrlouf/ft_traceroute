/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:27:36 by nponchon          #+#    #+#             */
/*   Updated: 2025/11/08 13:08:49 by nponchon         ###   ########.fr       */
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

char	*resolve_intermediate_hostname(char *addr_str)
{
	char	*hostname = NULL;
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	inet_pton(AF_INET, addr_str, &sa.sin_addr);

	char host[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&sa, sizeof(sa),
					host, sizeof(host), NULL, 0, 0) == 0) {
		hostname = ft_strdup(host);
	} else {
		hostname = ft_strdup("Unknown");
	}
	return hostname;
}

void	print_message(t_traceroute *t, char *addr_str)
{
	struct timeval	tv;
	float			time_ms;

	if (t->seq == 0) {
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(addr_str, STDOUT_FILENO);

		if (t->dnsresolve) {
			char *resolved = resolve_intermediate_hostname(addr_str);
			if (!resolved)
				error_exit("strdup");
			ft_putstr_fd(" (", STDOUT_FILENO);
			ft_putstr_fd(resolved, STDOUT_FILENO);
			ft_putstr_fd(")", STDOUT_FILENO);
			free(resolved);
		}
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