/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 14:39:02 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/27 16:19:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char		*ft_strdup(const char *s1)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(s1);
	if (!(dup = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	dup[len] = '\0';
	while (len-- > 0)
		dup[len] = s1[len];
	return (dup);
}
