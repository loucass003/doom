/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_rotate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:09:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:09:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>

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
