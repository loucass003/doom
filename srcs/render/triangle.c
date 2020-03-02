/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/02 15:34:57 by lloncham         ###   ########.fr       */
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

int					ft_max(int a, int b)
{
	return (a > b ? a : b);
}

typedef union	ut_color {
	uint32_t color;
	struct argbTag
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	} argb;
} ur_color;

void				scanline2(t_render_data data, t_pixel p, float t,
	t_vertex start, t_vertex end)
{
	t_vertex	vert;
	float		*buff;

	vert.pos.w = (1.0f - t) * start.pos.w + t * end.pos.w;
	buff = data.ctx->buffer + p.y * (int)S_WIDTH + p.x;
	if (vert.pos.w <= *buff)
	{
		float lt_color;
		float a;

		lt_color = (1.0f - t) * start.light_color + t * end.light_color;
		ur_color c;

		a = ft_max(AMBIANT_LIGHT, lt_color) / 255.0;
		if (data.mtl->texture_map_set)
		{
			float	w;
			int		x;
			int		y;

			w = 1. / vert.pos.w;
			vert.tex.x = (1.0f - t) * start.tex.x + t * end.tex.x;
			vert.tex.y = (1.0f - t) * start.tex.y + t * end.tex.y;

			x = (int)(ft_abs(((vert.tex.x > 0) - vert.tex.x * w)
				* (data.mtl->texture_map->width)))
				% (data.mtl->texture_map->width);

			y = (int)(ft_abs(((vert.tex.y > 0) - vert.tex.y * w)
				* (data.mtl->texture_map->height)))
				% (data.mtl->texture_map->height);

			x = vert.tex.x > 0 ? (data.mtl->texture_map->width - 1) - x : x;
			y = vert.tex.y < 0 ? (data.mtl->texture_map->height - 1) - y : y;
			if (data.mtl->texture_map_set)
				c.color = data.mtl->texture_map->pixels[(y
					* data.mtl->texture_map->width) + x];
			if (c.argb.a == 0)
				return ;
		}
		else if (data.mtl->material_color_set)
			c.color = data.mtl->material_color;
		if (!data.mtl->transparent)
		{
			c.argb.r *= a;
			c.argb.g *= a;
			c.argb.b *= a;
		}
		data.ctx->image->pixels[p.y * (int)S_WIDTH + p.x] = c.color;
		*buff = vert.pos.w;
	}

}

void				TexturedTriangle2(t_render_data data)
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
	{
		int	y_start;
		int	y_end;
		int	i;

		y_start = ft_max(data.min.y, ft_min(data.triangle.a.pos.y, data.max.y));
		y_end = ft_max(data.min.y, ft_min(data.triangle.b.pos.y, data.max.y));
		i = y_start - 1;
		while (++i < y_end)
		{
			t_vertex	start;
			t_vertex	end;

			start = vertex_add(data.triangle.a,
				vertex_mul_s(d1_step, (i - (int)data.triangle.a.pos.y)));
			end = vertex_add(data.triangle.a,
				vertex_mul_s(d2_step, (i - (int)data.triangle.a.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);

			float	tstep;
			int		x_start;
			int		x_end;
			float	t;
			int		j;

			tstep = 1.0f / (end.pos.x - start.pos.x);
			x_start = ft_max(data.min.x, ft_min(start.pos.x, data.max.x));
			x_end = ft_max(data.min.x, ft_min(end.pos.x, data.max.x));
			t = tstep * (x_start - start.pos.x);
			j = x_start - 1;
			while (++j < x_end)
			{
				scanline2(data, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}

	d1 = vertex_sub(data.triangle.c, data.triangle.b);
	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d1.pos.y)
	{
		int y_start;
		int y_end;
		int	i;

		y_start = ft_max(data.min.y, ft_min(data.triangle.b.pos.y, data.max.y));
		y_end = ft_max(data.min.y, ft_min(data.triangle.c.pos.y, data.max.y));

		i = y_start - 1;
		while (++i < y_end)
		{
			t_vertex start;
			t_vertex end;

			start = vertex_add(data.triangle.b,
				vertex_mul_s(d1_step, (i - (int)data.triangle.b.pos.y)));
			end = vertex_add(data.triangle.a,
				vertex_mul_s(d2_step, (i - (int)data.triangle.a.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);

			float	tstep;
			int		x_start;
			int		x_end;
			float	t;
			int		j;

			tstep = 1.0f / (end.pos.x - start.pos.x);
			x_start = ft_max(data.min.x, ft_min(start.pos.x, data.max.x));
			x_end = ft_max(data.min.x, ft_min(end.pos.x, data.max.x));
			t = tstep * (x_start - start.pos.x);
			j = x_start - 1;
			while (++j < x_end)
			{
				scanline2(data, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}
}

void				draw_triangle(t_render_data data)
{
	uint32_t	c;
	t_triangle	triangle;

	if (!data.mtl->wireframe)
		TexturedTriangle2(data);
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
