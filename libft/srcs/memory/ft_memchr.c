/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 14:02:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/17 14:50:24 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const int		*c_s = s;
	unsigned char	*b_s;
	int				repeated_c;
	int				mask;

	repeated_c = (c & 0xFF) | ((c & 0xFF) << 8);
	repeated_c |= repeated_c << 16;
	while (n >= 4)
	{
		mask = *c_s++ ^ repeated_c;
		if ((mask & 0xff) == 0)
			return ((void *)c_s - 4);
		if ((mask & 0xff00) == 0)
			return ((void *)c_s - 3);
		if ((mask & 0xff0000) == 0)
			return ((void *)c_s - 2);
		if ((mask & 0xff000000) == 0)
			return ((void *)c_s - 1);
		n -= 4;
	}
	b_s = (unsigned char *)c_s;
	while (n--)
		if (*b_s++ == (unsigned char)c)
			return ((void *)b_s - 1);
	return (NULL);
}
