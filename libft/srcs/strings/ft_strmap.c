/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:55:53 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/15 19:55:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmap(char const *s, char (*f)(char))
{
	char	*out;
	size_t	len;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	if (!(out = ft_strnew(len)))
		return (NULL);
	out[len] = '\0';
	while (len-- > 0)
		out[len] = f(s[len]);
	return (out);
}
