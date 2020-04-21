/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat4_mulv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 18:55:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/06 19:15:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>
#include <stdio.h>

t_vec3	ft_mat4_mulv(t_mat4 m, t_vec3 p)
{
	t_vec3	res;
	float	w;

	w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + m.a[3][3];
	res.x = (p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + m.a[0][3]);
	res.y = (p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + m.a[1][3]);
	res.z = (p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + m.a[2][3]);
	if (w != 0 && w != 1)
	{
		res.x /= w;
		res.y /= w;
		res.z /= w;
	}
	return (res);
}
