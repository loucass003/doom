/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat2_mul.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 18:56:30 by llelievr          #+#    #+#             */
/*   Updated: 2019/01/24 18:22:03 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_mat2		ft_mat2_mul(t_mat2 a, t_mat2 b)
{
	t_mat2	result;
	size_t	x;
	size_t	y;

	result = ft_mat2_identity();
	y = 0;
	while (y < 2)
	{
		x = 0;
		while (x < 2)
		{
			result.a[y][x] = a.a[y][0] * b.a[0][x]
				+ a.a[y][1] * b.a[1][x];
			x++;
		}
		y++;
	}
	return (result);
}
