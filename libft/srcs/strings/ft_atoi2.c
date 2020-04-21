/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:58:45 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/21 15:33:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <types.h>
#include <limits.h>

static t_bool		check_error(unsigned long out, char signe)
{
	if (out > (unsigned long)INT_MIN && signe == -1)
		return (TRUE);
	if (out > (unsigned long)INT_MAX && signe == 1)
		return (TRUE);
	return (FALSE);
}

int					ft_atoi2(const char *str, t_bool *error)
{
	unsigned long	out;
	char			signe;

	out = 0;
	signe = 1;
	while (*str == ' ' || (*str >= 7 && *str <= 13))
		str++;
	if (!(*str >= '0' && *str <= '9') && (*str != '-' && *str != '+'))
	{
		if (error)
			*error = TRUE;
		return (-1);
	}
	if (*str == '-' || *str == '+')
		signe = *str++ == '-' ? -1 : 1;
	while (*str >= '0' && *str <= '9')
		out = out * 10 + (*str++) - '0';
	if (check_error(out, signe))
	{
		if (error)
			*error = TRUE;
		return (-1);
	}
	return (out * signe);
}
