/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:03:04 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/17 14:48:42 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		if (n - i >= 8)
		{
			((uint64_t *)dst)[i / 8] = ((uint64_t *)src)[i / 8];
			i += 8;
		}
		else if (n - i >= 4)
		{
			((uint32_t *)dst)[i / 4] = ((uint32_t *)src)[i / 4];
			i += 4;
		}
		else if (n - i >= 2)
		{
			((uint16_t *)dst)[i / 2] = ((uint16_t *)src)[i / 2];
			i += 2;
		}
		else
		{
			((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
			i++;
		}
	return (dst);
}
