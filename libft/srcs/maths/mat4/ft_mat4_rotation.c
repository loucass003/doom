/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat4_rotation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 19:06:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/11 07:14:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

static t_mat4	s_rotate_x(float a)
{
	const float	c = cosf(a);
	const float s = sinf(a);

	return ((t_mat4)((t_mat4_data){
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1
	}));
}

static t_mat4	s_rotate_y(float a)
{
	const float c = cosf(a);
	const float s = sinf(a);

	return ((t_mat4)((t_mat4_data) {
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	}));
}

static t_mat4	s_rotate_z(float a)
{
	const float c = cosf(a);
	const float s = sinf(a);

	return ((t_mat4)((t_mat4_data) {
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}));
}

t_mat4			ft_mat4_rotation(t_vec3 angles)
{
	t_mat4	x;
	t_mat4	y;
	t_mat4	z;

	x = ft_mat4_identity();
	y = ft_mat4_identity();
	z = ft_mat4_identity();
	if (angles.x == 0 && angles.y == 0 && angles.z == 0)
		return (ft_mat4_identity());
	if (angles.x != 0)
		x = s_rotate_x(angles.x);
	if (angles.y != 0)
		y = s_rotate_y(angles.y);
	if (angles.z != 0)
		z = s_rotate_z(angles.z);
	return (ft_mat4_mul(ft_mat4_mul(x, y), z));
}
