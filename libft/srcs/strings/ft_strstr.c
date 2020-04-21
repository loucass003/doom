/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 14:58:21 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/13 16:31:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strstr(const char *haystack, const char *needle)
{
	size_t		len;

	len = ft_strlen(needle);
	if (len == 0)
		return ((char*)haystack);
	while (*haystack)
	{
		if (ft_memcmp(haystack, needle, len) == 0)
			return ((char*)haystack);
		haystack++;
	}
	return (NULL);
}
