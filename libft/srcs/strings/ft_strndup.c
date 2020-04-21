/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 14:39:02 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/25 22:14:42 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "strings.h"
#include <stdlib.h>

char		*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	char	*dup;

	len = n;
	if (!(dup = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	dup[len] = '\0';
	while (len-- > 0)
		dup[len] = s1[len];
	return (dup);
}
