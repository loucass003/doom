/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 16:02:31 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/09 16:59:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t len;

	len = ft_strlen(needle);
	if (len == 0)
		return ((char*)haystack);
	while (*haystack && n >= len)
	{
		if (ft_memcmp(haystack, needle, len) == 0)
			return ((char*)haystack);
		haystack++;
		n--;
	}
	return (NULL);
}
