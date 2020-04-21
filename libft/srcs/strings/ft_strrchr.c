/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 15:01:10 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 15:37:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char				*ft_strrchr(const char *s, int c)
{
	size_t			len;
	char			*str;

	str = (char*)s;
	len = ft_strlen(s);
	if (c == 0)
		return (str + len);
	while (len-- > 0)
	{
		if (str[len] == (unsigned char)c)
			return (str + len);
	}
	return (NULL);
}
