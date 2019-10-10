/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 17:50:27 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 04:42:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "maths/vec4.h"

t_vec4	vec3_to_4(t_vec3 v)
{
	return ((t_vec4){v.x, v.y, v.z, 1});
}

t_vec3	vec4_to_3w(t_vec4 v)
{
	return ((t_vec3){v.x / v.w, v.y / v.w, v.z / v.w});
}

t_vec3	vec4_to_3(t_vec4 v)
{
	return ((t_vec3){v.x, v.y, v.z});
}

t_vec4	mat4_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + p.w * m.a[3][3];
	return (r);
}

t_vec4		ft_vec4_mul_s(t_vec4 a, float v)
{
	a.x *= v;
	a.y *= v;
	a.z *= v;
	a.w *= v;
	return (a);
}

t_vec4		ft_vec4_div_s(t_vec4 a, float v)
{
	a.x /= v;
	a.y /= v;
	a.z /= v;
	a.w /= v;
	return (a);
}

t_vec4		ft_vec4_div(t_vec4 a, t_vec4 b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	a.w /= b.w;
	return (a);
}

t_vec4		ft_vec4_mul(t_vec4 a, t_vec4 b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	a.w *= b.w;
	return (a);
}

t_vec4		ft_vec4_sub(t_vec4 a, t_vec4 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return (a);
}

t_vec4		ft_vec4_sub_s(t_vec4 a, float v)
{
	a.x -= v;
	a.y -= v;
	a.z -= v;
	a.w -= v;
	return (a);
}


t_vec4		ft_vec4_add(t_vec4 a, t_vec4 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return (a);
}

t_vec4	ft_vec4_interpolate(t_vec4 a, t_vec4 b, float alpha)
{
	return (ft_vec4_add(a, ft_vec4_mul_s(ft_vec4_sub(b, a), alpha)));
}

static t_vec3	vec3_rotate_x(t_vec3 v, float angle)
{
	const float cos_a = cosf(angle);
	const float sin_a = sinf(angle);

	return ((t_vec3){
		v.x,
		v.y * cos_a - v.z * sin_a,
		v.y * sin_a + v.z * cos_a
	});
}

static t_vec3	vec3_rotate_y(t_vec3 v, float angle)
{
	const float cos_a = cosf(angle);
	const float sin_a = sinf(angle);

	return ((t_vec3){
		v.x * cos_a + v.z * sin_a,
		v.y,
		-v.x * sin_a + v.z * cos_a
	});
}

static t_vec3	vec3_rotate_z(t_vec3 v, float angle)
{
	const float cos_a = cosf(angle);
	const float sin_a = sinf(angle);

	return ((t_vec3){
		v.x * cos_a - v.y * sin_a,
		v.x * sin_a + v.y * cos_a,
		v.z
	});
}

t_vec3			vec3_rotate(t_vec3 v, t_vec3 rot)
{
	v = vec3_rotate_x(v, rot.x);
	v = vec3_rotate_y(v, rot.y);
	v = vec3_rotate_z(v, rot.z);
	return (v);
}
