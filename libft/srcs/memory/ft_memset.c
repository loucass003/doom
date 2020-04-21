/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 14:14:30 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/25 22:14:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dst, int car, size_t len)
{
	size_t			i;
	const uint16_t	b = ((uint16_t)car << 8 | (uint16_t)car);
	const uint32_t	c = ((uint32_t)b << 16 | (uint32_t)b);
	const uint64_t	d = ((uint64_t)c << 32 | (uint64_t)c);

	i = 0;
	while (i < len)
		if (len - i >= 8)
		{
			((uint64_t *)dst)[i / 8] = d;
			i += 8;
		}
		else if (len - i >= 4)
		{
			((uint32_t *)dst)[i / 4] = c;
			i += 4;
		}
		else if (len - i >= 2)
		{
			((uint16_t *)dst)[i / 2] = b;
			i += 2;
		}
		else
			((uint8_t *)dst)[i++] = car;
	return (dst); 
}
