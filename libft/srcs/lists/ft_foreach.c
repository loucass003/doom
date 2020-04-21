/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_foreach.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 19:08:17 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/20 16:26:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_foreach(void **tab, size_t len, void (*f)(void **))
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (i++ < len)
		f(&(*tab++));
}
