/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 08:06:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/21 02:06:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool		get_separating_plane(t_vec3 pos, t_vec3 plane, t_obb_box a, t_obb_box b)
{
	return (fabs(ft_vec3_dot(pos, plane)) 
		> (fabs(ft_vec3_dot(ft_vec3_mul_s(a.x_axis, a.half_size.x), plane)))
		+ (fabs(ft_vec3_dot(ft_vec3_mul_s(a.y_axis, a.half_size.y), plane)))
		+ (fabs(ft_vec3_dot(ft_vec3_mul_s(a.z_axis, a.half_size.z), plane)))
		+ (fabs(ft_vec3_dot(ft_vec3_mul_s(b.x_axis, b.half_size.x), plane)))
		+ (fabs(ft_vec3_dot(ft_vec3_mul_s(b.y_axis, b.half_size.y), plane)))
		+ (fabs(ft_vec3_dot(ft_vec3_mul_s(b.z_axis, b.half_size.z), plane)))
	);
}

t_bool		get_obb_collision(t_obb_box a, t_obb_box b) 
{
	const	t_vec3	pos = ft_vec3_sub(b.pos, a.pos);

	return (!((get_separating_plane(pos, a.x_axis, a, b) 
		|| get_separating_plane(pos, a.y_axis, a, b)
		|| get_separating_plane(pos, a.z_axis, a, b)
		|| get_separating_plane(pos, b.x_axis, a, b)
		|| get_separating_plane(pos, b.y_axis, a, b)
		|| get_separating_plane(pos, b.z_axis, a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.x_axis, b.x_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.x_axis, b.y_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.x_axis, b.z_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.y_axis, b.x_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.y_axis, b.y_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.y_axis, b.z_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.z_axis, b.x_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.z_axis, b.y_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.z_axis, b.z_axis), a, b)
	)));
}

static t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	//r.w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + p.w * m.a[3][3];
	return (r);
}
