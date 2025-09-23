/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:39:47 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/23 18:25:00 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_traceroute.h"

void    parse_arg(char *arg)
{
    if (ft_strcmp(arg, "--help") == 0) {
        printf(HELP_MSG);
        exit(EXIT_SUCCESS);
    } else if (ft_strcmp(arg, "--usage") == 0) {
        printf(USAGE_MSG);
        exit(EXIT_SUCCESS);
    } else
        exit(EXIT_SUCCESS);

    // TODO: parse options for the bonus
    // parse_options();

}
