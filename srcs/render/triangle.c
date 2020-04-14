/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 23:23:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths/vertex.h"
#include "maths/triangle.h"
#include "doom.h"
#include "render.h"
#include "image.h"
#include <pthread.h>
#include <stdatomic.h>

static inline void	swap(t_vertex *a, t_vertex *b)
{
	t_vertex t;

	t = *a;
	*a = *b;
	*b = t;
}

void				scanline(t_render_data data, t_vertex step[2],
	t_vertex v[2], int i)
{
	t_vertex	v_bounds[2];
	int			x_bounds[2];
	float		tstep;
	float		t;
	int			j;

	v_bounds[0] = vertex_add(v[0],
		vertex_mul_s(step[0], (i - (int)v[0].pos.y)));
	v_bounds[1] = vertex_add(v[1],
		vertex_mul_s(step[1], (i - (int)v[1].pos.y)));
	if (v_bounds[0].pos.x > v_bounds[1].pos.x)
		swap(&v_bounds[0], &v_bounds[1]);
	tstep = 1.0f / (v_bounds[1].pos.x - v_bounds[0].pos.x);
	x_bounds[0] = ft_max(data.min.x, ft_min(v_bounds[0].pos.x, data.max.x));
	x_bounds[1] = ft_max(data.min.x, ft_min(v_bounds[1].pos.x, data.max.x));
	t = tstep * (x_bounds[0] - v_bounds[0].pos.x);
	j = x_bounds[0] - 1;
	while (++j < x_bounds[1])
	{
		draw_pixel(data, (t_pixel_data) { j, i, t, v_bounds[0], v_bounds[1] });
		t += tstep;
	}
}

void				scan_row(t_render_data d, t_vertex step[2],
	t_vertex v[2], t_vec2 bounds)
{
	int	y_start;
	int	y_end;
	int	i;

	y_start = ft_max(d.min.y, ft_min(bounds.x, d.max.y));
	y_end = ft_max(d.min.y, ft_min(bounds.y, d.max.y));
	i = y_start - 1;
	while (++i < y_end)
		scanline(d, step, v, i);
}

void				textured_triangle(t_render_data data)
{
	t_vertex	d1;
	t_vertex	d2;
	t_vertex	d1_step;
	t_vertex	d2_step;

	d1 = vertex_sub(data.triangle.b, data.triangle.a);
	d2 = vertex_sub(data.triangle.c, data.triangle.a);
	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d2.pos.y)
		d2_step = vertex_div_s(d2, fabsf(d2.pos.y));
	if (d1.pos.y)
		scan_row(data, (t_vertex[2]){ d1_step, d2_step },
			(t_vertex[2]){ data.triangle.a, data.triangle.a },
			(t_vec2){ data.triangle.a.pos.y, data.triangle.b.pos.y });
	d1 = vertex_sub(data.triangle.c, data.triangle.b);
	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d1.pos.y)
		scan_row(data, (t_vertex[2]){ d1_step, d2_step },
			(t_vertex[2]){ data.triangle.b, data.triangle.a },
			(t_vec2){ data.triangle.b.pos.y, data.triangle.c.pos.y });
}

void				draw_triangle(t_render_data data)
{
	uint32_t	c;
	t_triangle	triangle;

	if (!data.mtl->wireframe)
		textured_triangle(data);
	if (data.mtl->wireframe)
	{
		c = data.mtl->material_color_set
			? data.mtl->material_color : 0xFFFFFFFF;
		triangle = data.triangle;
		draw_line_zbuff(data.ctx, triangle.a.pos, triangle.b.pos, c);
		draw_line_zbuff(data.ctx, triangle.b.pos, triangle.c.pos, c);
		draw_line_zbuff(data.ctx, triangle.c.pos, triangle.a.pos, c);
	}
}
