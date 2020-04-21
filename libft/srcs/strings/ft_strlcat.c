/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 12:44:17 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/14 14:37:37 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlcat(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (dst[i] && i < n)
		i++;
	if (i == n)
		return (i + ft_strlen(src));
	while (*src && i < n - 1)
		dst[i++] = *src++;
	dst[i] = '\0';
	while (*src++)
		i++;
	return (i);
}
