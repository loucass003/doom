/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:31:48 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "maths/vertex.h"
#include "maths/triangle.h"
#include "doom.h"

static void		clip_2(t_render_context *ctx, t_mtl *mtl, t_triangle t)
{
	const float		alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const float		alpha_b = (NEAR_CLIP - t.b.pos.z) / (t.c.pos.z - t.b.pos.z);
	const t_vertex	v_a = vertex_interpolate(t.a, t.c, alpha_a);
	const t_vertex	v_b = vertex_interpolate(t.b, t.c, alpha_b);

	post_process_triangle(ctx, mtl, (t_triangle){ v_a, v_b, t.c });
}

static void		clip_1(t_render_context *ctx, t_mtl *mtl, t_triangle t)
{
	const float		alpha_a = (NEAR_CLIP - t.a.pos.z) / (t.b.pos.z - t.a.pos.z);
	const float		alpha_b = (NEAR_CLIP - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const t_vertex	v_a = vertex_interpolate(t.a, t.b, alpha_a);
	const t_vertex	v_b = vertex_interpolate(t.a, t.c, alpha_b);

	post_process_triangle(ctx, mtl, (t_triangle){ v_a, t.b, t.c });
	post_process_triangle(ctx, mtl, (t_triangle){ v_b, v_a, t.c });
}

void			process_triangle(t_render_context *ctx, t_mtl *mtl,
	t_triangle t)
{
	if (t.a.pos.z > FAR_CULL && t.b.pos.z > FAR_CULL && t.c.pos.z > FAR_CULL)
		return ;
	if (t.a.pos.z < NEAR_CLIP && t.b.pos.z < NEAR_CLIP && t.c.pos.z < NEAR_CLIP)
		return ;
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
		post_process_triangle(ctx, mtl, t);
}
