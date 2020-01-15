/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 03:53:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "maths/vertex.h"
#include "maths/triangle.h"
#include "doom.h"

static t_vertex	transform(t_vertex v)
{
	const float	z_inv = -1. / v.pos.z;
	const float w_inv = 1 / v.pos.w;

	v.pos.x = (v.pos.x * z_inv + 1) * S_WIDTH_2;
	v.pos.y = (v.pos.y * z_inv + 1) * S_HEIGHT_2;
	v.tex = ft_vec2_div_s(v.tex, v.pos.w);
	v.pos.w = w_inv;
	return v;
}

static void 	clip_2(t_render_context *ctx, t_triangle t)
{
	const float	alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR_CLIP - t.b.pos.z) / (t.c.pos.z - t.b.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.c, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.b, t.c, alpha_b);

	post_process_triangle(ctx, (t_triangle){ v_a, v_b, t.c, t.face_index, t.r });
}

static void 	clip_1(t_render_context *ctx, t_triangle t)
{
	const float	alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.b.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.b, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.a, t.c, alpha_b);
	
	post_process_triangle(ctx, (t_triangle){ v_a, t.b, t.c, t.face_index, t.r });
	post_process_triangle(ctx, (t_triangle){ v_b, v_a, t.c, t.face_index, t.r });
}

void	process_triangle(t_render_context *ctx, t_triangle t)
{
	if (t.a.pos.z > FAR_CULL && t.b.pos.z > FAR_CULL && t.c.pos.z > FAR_CULL)
		return;
	if (t.a.pos.z < NEAR_CLIP && t.b.pos.z < NEAR_CLIP && t.c.pos.z < NEAR_CLIP)
		return;
	
	if (t.a.pos.z < NEAR_CLIP)
	{
		if (t.b.pos.z < NEAR_CLIP)
			clip_2(ctx, t);
		else if (t.c.pos.z < NEAR_CLIP)
			clip_2(ctx, (t_triangle){t.a, t.c, t.b, t.face_index, t.r});
		else
			clip_1(ctx, t);
	}
	else if (t.b.pos.z < NEAR_CLIP)
	{
		if (t.c.pos.z < NEAR_CLIP)
			clip_2(ctx, (t_triangle){t.b, t.c, t.a, t.face_index, t.r});
		else
			clip_1(ctx, (t_triangle){t.b, t.a, t.c, t.face_index, t.r});
	}
	else if (t.c.pos.z < NEAR_CLIP)
		clip_1(ctx, (t_triangle){t.c, t.a, t.b, t.face_index, t.r});
	else
	{
		post_process_triangle(ctx, t);
	}
}


void	post_process_triangle(t_render_context *ctx, t_triangle t)
{
	t.a = transform(t.a);
	t.b = transform(t.b);
	t.c = transform(t.c);

	draw_triangle(ctx, t);
}

t_vec3	get_triangle_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2)
{
	const t_vec3 a = ft_vec3_sub(p1, p0);
	const t_vec3 b = ft_vec3_sub(p2, p0);
	
	return (ft_vec3_norm(ft_vec3_cross(a, b)));
}
