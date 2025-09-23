/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 18:00:42 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/23 18:05:08 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strcmp(const char *s1, const char *s2)
{
    if (s1 == NULL || s2 == NULL)
        return (-1);
    while (*s1 && *s2) {
        s1++;
        s2++;
    }
    return ((unsigned char) *s1 - (unsigned char) *s2);
}