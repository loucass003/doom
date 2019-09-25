/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/25 14:55:23 by llelievr         ###   ########.fr       */
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
	v.pos.y = (v.pos.y * z_inv + 0.5) * S_HEIGHT_2 * ASPECT_INV;
	v.tex = ft_vec2_div_s(v.tex, v.pos.w);
	v.pos.w = w_inv;
	return v;
}

static void 	clip_2(t_render_context *ctx, t_mtl *mtl, t_triangle t)
{
	const float	alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR_CLIP - t.b.pos.z) / (t.c.pos.z - t.b.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.c, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.b, t.c, alpha_b);

	post_process_triangle(ctx, mtl, (t_triangle){ v_a, v_b, t.c });
}

static void 	clip_1(t_render_context *ctx, t_mtl *mtl, t_triangle t)
{
	const float	alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.b.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.b, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.a, t.c, alpha_b);
	
	post_process_triangle(ctx, mtl, (t_triangle){ v_a, t.b, t.c });
	post_process_triangle(ctx, mtl, (t_triangle){ v_b, v_a, t.c });
}

void	process_triangle(t_render_context *ctx, t_mtl *mtl, t_triangle t)
{
	// t.a.pos = mat4_mulv4(ctx->camera->projection, t.a.pos);
	// t.b.pos = mat4_mulv4(ctx->camera->projection, t.b.pos);
	// t.c.pos = mat4_mulv4(ctx->camera->projection, t.c.pos);
	if (t.a.pos.y < t.a.pos.w && t.b.pos.y < t.b.pos.w && t.c.pos.y < t.c.pos.w)
		return;
	if (t.a.pos.y > -t.a.pos.w && t.b.pos.y > -t.b.pos.w && t.c.pos.y > -t.c.pos.w)
		return;
	if (t.a.pos.x < t.a.pos.w && t.b.pos.x < t.b.pos.w && t.c.pos.x < t.c.pos.w)
		return;
	if (t.a.pos.x > -t.a.pos.w && t.b.pos.x > -t.b.pos.w && t.c.pos.x > -t.c.pos.w)
		return;
	if (t.a.pos.z > FAR_CULL && t.b.pos.z > FAR_CULL && t.c.pos.z > FAR_CULL)
		return;
	if (t.a.pos.z < NEAR_CLIP && t.b.pos.z < NEAR_CLIP && t.c.pos.z < NEAR_CLIP)
		return;
	
	if (t.a.pos.z < NEAR_CLIP)
	{
		if (t.b.pos.z < NEAR_CLIP)
			clip_2(ctx, mtl, t);
		else if (t.c.pos.z < NEAR_CLIP)
			clip_2(ctx, mtl, (t_triangle){t.a, t.c, t.b});
		else
			clip_1(ctx, mtl, t);
	}
	else if (t.b.pos.z < NEAR_CLIP)
	{
		if (t.c.pos.z < NEAR_CLIP)
			clip_2(ctx, mtl, (t_triangle){t.b, t.c, t.a});
		else
			clip_1(ctx, mtl, (t_triangle){t.b, t.a, t.c});
	}
	else if (t.c.pos.z < NEAR_CLIP)
		clip_1(ctx, mtl, (t_triangle){t.c, t.a, t.b});
	else
	{
		post_process_triangle(ctx, mtl, t);
	}
}

void	post_process_triangle(t_render_context *ctx, t_mtl *mtl, t_triangle t)
{
	t.a = transform(t.a);
	t.b = transform(t.b);
	t.c = transform(t.c);

	t_render_data data = (t_render_data) {
		.mtl = mtl,
		.triangle = t
	};
	
	draw_triangle(ctx, data);
}

t_vec3	get_triangle_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2)
{
	const t_vec3 a = ft_vec3_sub(p1, p0);
	const t_vec3 b = ft_vec3_sub(p2, p0);
	
	return (ft_vec3_norm(ft_vec3_cross(a, b)));
}
