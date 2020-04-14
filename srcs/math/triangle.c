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

static t_vertex	transform(t_vertex v)
{
	const float	z_inv = -1. / v.pos.z;
	const float w_inv = 1 / v.pos.w;

	v.pos.x = (v.pos.x * z_inv + 1) * S_WIDTH_2;
	v.pos.y = (v.pos.y * z_inv + 1) * S_HEIGHT_2;
	v.tex = ft_vec2_div_s(v.tex, v.pos.w);
	v.pos.w = w_inv;
	return (v);
}

void			post_process_triangle(t_render_context *ctx, t_mtl *mtl,
	t_triangle t)
{
	t_render_data data;

	data = (t_render_data) {
		.mtl = mtl,
		.triangle = (t_triangle){
			.a = transform(t.a),
			.b = transform(t.b),
			.c = transform(t.c)},
		.ctx = ctx
	};
	add_data_to_threads(&ctx->doom->threads, data);
}

t_vec3			get_triangle_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2)
{
	const t_vec3 a = ft_vec3_sub(p1, p0);
	const t_vec3 b = ft_vec3_sub(p2, p0);

	return (ft_vec3_norm(ft_vec3_cross(a, b)));
}
