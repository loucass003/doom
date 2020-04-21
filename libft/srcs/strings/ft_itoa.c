/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 14:19:14 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 16:40:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	d_len(int n)
{
	size_t		len;

	len = (n == 0);
	n *= (n < 0 ? 1 : -1);
	while (n < 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char			*ft_itoa(int n)
{
	size_t		len;
	char		*out;

	len = d_len(n) + (n < 0);
	if (!(out = ft_strnew(len)))
		return (NULL);
	if (n == 0)
		*out = '0';
	if (n < 0)
		*out = '-';
	n *= (n < 0 ? 1 : -1);
	while (n < 0)
	{
		out[--len] = (n % 10) * -1 + '0';
		n /= 10;
	}
	return (out);
}
