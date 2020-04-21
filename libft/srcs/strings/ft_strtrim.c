/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 14:39:19 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 17:03:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	s_isspace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

char		*ft_strtrim(char const *s)
{
	size_t	end;
	size_t	start;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	end = 0;
	start = 0;
	while (s_isspace(*(s + start)))
		start++;
	while (s_isspace(s[len - 1 - end]))
		end++;
	if (start == len)
		return (ft_strnew(0));
	return (ft_strsub(s, start, len - end - start));
}
