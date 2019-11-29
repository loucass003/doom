/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:52:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/29 17:11:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

t_vec3		vec3_clamp_s(t_vec3 v, float min, float max)
{
	v.x = fmax(min, fmin(max, v.x));
	v.y = fmax(min, fmin(max, v.y));
	v.z = fmax(min, fmin(max, v.z));
	return v;
}

t_vec3		vec3_add_s(t_vec3 v, float s)
{
	v.x += s;
	v.y += s;
	v.z += s;
	return v;
}

t_vec3		vec3_sub_s(t_vec3 v, float s)
{
	v.x -= s;
	v.y -= s;
	v.z -= s;
	return v;
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


t_vec3	ft_vec3_interpolate(t_vec3 a, t_vec3 b, float alpha)
{
	return (ft_vec3_add(a, ft_vec3_mul_s(ft_vec3_sub(b, a), alpha)));
}