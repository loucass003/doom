/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat4_mul.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 18:56:30 by llelievr          #+#    #+#             */
/*   Updated: 2018/12/07 16:25:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_mat4		ft_mat4_mul(t_mat4 a, t_mat4 b)
{
	t_mat4	result;
	size_t	x;
	size_t	y;

	result = ft_mat4_identity();
	y = 0;
	while (y < 4)
	{
		x = 0;
		while (x < 4)
		{
			result.a[y][x] = a.a[y][0] * b.a[0][x]
				+ a.a[y][1] * b.a[1][x]
				+ a.a[y][2] * b.a[2][x]
				+ a.a[y][3] * b.a[3][x];
			x++;
		}
		y++;
	}
	return (result);
}
