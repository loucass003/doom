/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 16:28:50 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 16:40:08 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int					ft_atoi(const char *str)
{
	unsigned long	out;
	char			signe;

	out = 0;
	signe = 1;
	while (*str == ' ' || (*str >= 7 && *str <= 13))
		str++;
	if (!(*str >= '0' && *str <= '9') && (*str != '-' && *str != '+'))
		return (0);
	if (*str == '-' || *str == '+')
		signe = *str++ == '-' ? -1 : 1;
	while (*str >= '0' && *str <= '9')
		out = out * 10 + (*str++) - '0';
	if (out > 9223372036854775807 && signe == -1)
		return (0);
	if (out > 9223372036854775806 && signe == 1)
		return (-1);
	return (out * signe);
}
