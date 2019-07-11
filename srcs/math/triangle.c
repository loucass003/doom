/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/11 17:52:37 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_vertex	transform(t_vertex v)
{
	const float	z_inv = -1. / v.pos.z;
	const float w_inv = 1 / v.pos.w;

	v.pos.x = (v.pos.x * z_inv + 1.) * S_WIDTH_2;
	v.pos.y = (v.pos.y * z_inv + 1.) * S_HEIGHT_2 * ASPECT_INV;
	v.tex = ft_vec2_div_s(v.tex, v.pos.w);
	v.pos.w = w_inv;
	return v;
}

static void 	clip_2(t_doom *doom, t_mtl *mtl, t_triangle t)
{
	const float	alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR_CLIP - t.b.pos.z) / (t.c.pos.z - t.b.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.c, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.b, t.c, alpha_b);

	post_process_triangle(doom, mtl, (t_triangle){ v_a, v_b, t.c });
}

static void 	clip_1(t_doom *doom, t_mtl *mtl, t_triangle t)
{
	const float	alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.b.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.b, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.a, t.c, alpha_b);

	post_process_triangle(doom, mtl, (t_triangle){ v_a, t.b, t.c });
	post_process_triangle(doom, mtl, (t_triangle){ v_b, v_a, t.c });
}

void	process_triangle(t_doom *doom, t_mtl *mtl, t_triangle t)
{
	t.a.pos = mat4_mulv4(doom->player.projection, t.a.pos);
	t.b.pos = mat4_mulv4(doom->player.projection, t.b.pos);
	t.c.pos = mat4_mulv4(doom->player.projection, t.c.pos);
	if (t.a.pos.x < t.a.pos.w && t.b.pos.x < t.b.pos.w && t.c.pos.x < t.c.pos.w)
		return;
	if (t.a.pos.x > -t.a.pos.w && t.b.pos.x > -t.b.pos.w && t.c.pos.x > -t.c.pos.w)
		return;
	if (mtl->material_color_set && t.a.pos.z > 10 && t.b.pos.z > 10 && t.c.pos.z > 10)
		return;
	if (t.a.pos.z < NEAR_CLIP && t.b.pos.z < NEAR_CLIP && t.c.pos.z < NEAR_CLIP)
		return;
	if (t.a.pos.z < NEAR_CLIP)
	{
		if (t.b.pos.z < NEAR_CLIP)
			clip_2(doom, mtl, t);
		else if (t.c.pos.z < NEAR_CLIP)
			clip_2(doom, mtl, (t_triangle){t.a, t.c, t.b});
		else
			clip_1(doom, mtl, t);
	}
	else if (t.b.pos.z < NEAR_CLIP)
	{
		if (t.c.pos.z < NEAR_CLIP)
			clip_2(doom, mtl, (t_triangle){t.b, t.c, t.a});
		else
			clip_1(doom, mtl, (t_triangle){t.b, t.a, t.c});
	}
	else if (t.c.pos.z < NEAR_CLIP)
		clip_1(doom, mtl, (t_triangle){t.c, t.a, t.b});
	else
	{
		post_process_triangle(doom, mtl, t);
	}
}

void	post_process_triangle(t_doom *doom, t_mtl *mtl, t_triangle t)
{
	t.a = transform(t.a);
	t.b = transform(t.b);
	t.c = transform(t.c);
	draw_triangle(doom, t, mtl);
}
