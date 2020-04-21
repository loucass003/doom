/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 13:58:57 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/21 16:29:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_pow(int n, int pow)
{
	int	out;

	if (pow == 1)
		return (n);
	if (pow == 0)
		return (1);
	out = n;
	while (--pow > 0)
		out *= n;
	return (out);
}
