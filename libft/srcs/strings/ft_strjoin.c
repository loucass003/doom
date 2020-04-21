/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 14:18:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/01/31 13:43:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strjoin(char const *s1, char const *s2)
{
	size_t		len;
	size_t		s1_len;
	char		*out;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	len = s1_len + ft_strlen(s2);
	if (!(out = (char *)ft_strnew(len)))
		return (NULL);
	out = ft_strcpy(out, (char *)s1);
	return (ft_strcat(out + s1_len, (char*)s2) - s1_len);
}
