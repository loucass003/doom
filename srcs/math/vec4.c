/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 17:50:27 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:13:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "maths/vec4.h"

t_vec4	mat4_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + p.w * m.a[3][3];
	return (r);
}

t_vec4	ft_vec4_interpolate(t_vec4 a, t_vec4 b, float alpha)
{
	return (ft_vec4_add(a, ft_vec4_mul_s(ft_vec4_sub(b, a), alpha)));
}

float	ft_vec4_dot(t_vec4 a, t_vec4 b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

t_vec4	ft_vec4_norm(t_vec4 v)
{
	const float	len = 1. / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return ((t_vec4) { v.x * len, v.y * len, v.z * len, v.w * len });
}
