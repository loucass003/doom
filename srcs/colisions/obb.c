/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 08:06:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/18 04:00:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <assert.h>

void		compute_polygon_obb(t_polygon *poly)
{
	const	t_vec3		n = get_polygon_normal(poly);
	t_bounds3	bounds = get_polygon_bounds(poly);
	t_vec3			w;
	t_vec3			u;

	assert(poly->vertices->len >= 3);
	w = ft_vec3_norm(ft_vec3_sub(poly->vertices->vertices[0],
		poly->vertices->vertices[1]));
	u = ft_vec3_cross(w, n);
	printf ("POLY %d\n", poly->type);
	printf ("u %f %f %f\n", u.x, u.y, u.z);
	printf ("w %f %f %f\n", w.x, w.y, w.z);
	printf ("n %f %f %f\n", n.x, n.y, n.z);

	poly->obb = (t_obb_box){
		.pos = ft_vec3_add(bounds.pos, ft_vec3_div_s(bounds.size, 2)),
		.half_size = ft_vec3_div_s(bounds.size, 2),
		.x_axis = {1, 0, 0},
		.y_axis = {0, 1, 0},
		.z_axis = {0, 0, 1},
	};
}

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

static t_vec4	transform(t_vec4 v)
{
	const float	z_inv = -1. / v.z;
	const float w_inv = 1 / v.w;

	v.x = (v.x * z_inv + 1.) * S_WIDTH_2;
	v.y = (v.y * z_inv + 1.) * S_HEIGHT_2 * ASPECT_INV;
	v.w = w_inv;
	return v;
}

void	draw_obb(t_doom *doom, t_obb_box obb)
{
	t_vec3	points[8] = {
		{ obb.pos.x - obb.half_size.x, obb.pos.y - obb.half_size.y, obb.pos.z - obb.half_size.z },
		{ obb.pos.x + obb.half_size.x, obb.pos.y - obb.half_size.y, obb.pos.z - obb.half_size.z },
		{ obb.pos.x - obb.half_size.x, obb.pos.y + obb.half_size.y, obb.pos.z - obb.half_size.z },
		{ obb.pos.x + obb.half_size.x, obb.pos.y + obb.half_size.y, obb.pos.z - obb.half_size.z },
		{ obb.pos.x - obb.half_size.x, obb.pos.y - obb.half_size.y, obb.pos.z + obb.half_size.z },
		{ obb.pos.x + obb.half_size.x, obb.pos.y - obb.half_size.y, obb.pos.z + obb.half_size.z },
		{ obb.pos.x - obb.half_size.x, obb.pos.y + obb.half_size.y, obb.pos.z + obb.half_size.z },
		{ obb.pos.x + obb.half_size.x, obb.pos.y + obb.half_size.y, obb.pos.z + obb.half_size.z }
	};
	t_mat4 m =  (t_mat4)((t_mat4_data){
		obb.x_axis.x, obb.y_axis.x, obb.z_axis.x, 0,
		obb.x_axis.y, obb.y_axis.y, obb.z_axis.y, 0,
		obb.x_axis.z, obb.y_axis.z, obb.z_axis.z, 0,
		0, 0, 0, 1
	});
	for (int i = 0; i < 8; i++)
	{
		t_vec4 p = mat4_mulv4(doom->player.projection, mat43_mulv4(doom->player.matrix, vec3_to_4(points[i])));
		t_vec4 p_next = mat4_mulv4(doom->player.projection, mat43_mulv4(doom->player.matrix, vec3_to_4(points[(i + 1) % 8])));
		if (p.z < NEAR_CLIP || p_next.z < NEAR_CLIP)
			continue;
		p = transform(p);
		p_next = transform(p_next);
		draw_line(&doom->screen, (t_pixel){p.x, p.y, 0xFFFF}, (t_pixel){p_next.x, p_next.y, 0xFFFFFF});
	}
}