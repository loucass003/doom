/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/15 04:33:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths/vertex.h"
#include "maths/triangle.h"
#include "doom.h"
#include "render.h"
#include <pthread.h>

static inline void	swap(t_vertex *a, t_vertex *b)
{
	t_vertex t = *a;
	*a = *b;
	*b = t;
}

int		ft_max(int a, int b)
{
	return (a > b ? a : b);
}

typedef union ut_color {
	uint32_t color;
	struct argbTag
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
        uint8_t a;
	} argb;
	t_color	color_t;
} ur_color;

void scanline2(t_render_context *ctx, t_mtl *mtl, t_pixel p, float t, t_vertex start, t_vertex end)
{
	t_vertex	vert;
	float		*buff;

	vert.pos.w = (1.0f - t) * start.pos.w + t * end.pos.w;
	buff = ctx->buffer + p.y * (int)S_WIDTH + p.x;
	// if (vert.pos.w > -25)
	// 	return ;
	if (vert.pos.w <= *buff)
	{
		*buff = vert.pos.w;
		float lt_color = (1.0f - t) * start.light_color + t * end.light_color;
		ur_color c;
		uint8_t a = ft_max(AMBIANT_LIGHT, lt_color);		
		if (mtl->lightmap || mtl->texture_map_set)
		{
			float w = 1. / vert.pos.w;
			vert.tex.x = (1.0f - t) * start.tex.x + t * end.tex.x;
			vert.tex.y = (1.0f - t) * start.tex.y + t * end.tex.y;
			int x = (int)(fabsf((vert.tex.x > 0 ? 1 - vert.tex.x * w : vert.tex.x * w) * (mtl->texture_map->w))) % (mtl->texture_map->w);
			int y = (int)(fabsf((vert.tex.y > 0 ? 1 - vert.tex.y * w : vert.tex.y * w) * (mtl->texture_map->h))) % (mtl->texture_map->h);

			x = vert.tex.x > 0 ? (mtl->texture_map->w - 1) - x : x;
			y = vert.tex.y < 0 ? (mtl->texture_map->h - 1) - y : y;
			if (mtl->lightmap)
				a = mtl->lightmap[y * (mtl->texture_map->w) + x];
			if (mtl->texture_map_set)
				c.color = get_surface_pixel(mtl->texture_map, x, y);
		}
		else if (mtl->material_color_set)
			c.color = mtl->material_color;
		if (!mtl->transparent)
			c.argb.a = a;
		ctx->image->pixels[p.y * (int)S_WIDTH + p.x] = c.color;
	}
}

// typedef struct s_data
// {
// 	int			i;
// 	t_vertex	start;
// 	t_vertex	end;
// 	t_doom		*doom;
// 	t_mtl		*mtl;
// }				t_data;

void TexturedTriangle2(t_render_context *ctx, t_render_data data)
{
	if (data.triangle.b.pos.y < data.triangle.a.pos.y)
		swap(&data.triangle.a, &data.triangle.b);
	if (data.triangle.c.pos.y < data.triangle.a.pos.y)
		swap(&data.triangle.a, &data.triangle.c);
	if (data.triangle.c.pos.y < data.triangle.b.pos.y)
		swap(&data.triangle.b, &data.triangle.c);
	t_vertex	d1 = vertex_sub(data.triangle.b, data.triangle.a);
	t_vertex	d2 = vertex_sub(data.triangle.c, data.triangle.a);
	t_vertex	d1_step;
	t_vertex	d2_step;

	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d2.pos.y)
		d2_step = vertex_div_s(d2, fabsf(d2.pos.y));
	if (d1.pos.y)
	{
		int y_start = ft_max(0, ft_min(data.triangle.a.pos.y, S_HEIGHT));
		int y_end = ft_max(0, ft_min(data.triangle.b.pos.y, S_HEIGHT));
		for (int i = y_start; i < y_end; i++)
		{
			t_vertex start = vertex_add(data.triangle.a, vertex_mul_s(d1_step, (i - (int)data.triangle.a.pos.y)));
			t_vertex end = vertex_add(data.triangle.a, vertex_mul_s(d2_step, (i - (int)data.triangle.a.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);
			float tstep = 1.0f / (end.pos.x - start.pos.x);
			int x_start = ft_max(0, ft_min(start.pos.x, S_WIDTH));
			int x_end = ft_max(0, ft_min(end.pos.x, S_WIDTH));
			float t = tstep * (x_start - start.pos.x);
			for (int j = x_start; j < x_end; j++)
			{
				scanline2(ctx, data.mtl, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}

	d1 = vertex_sub(data.triangle.c, data.triangle.b);
	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d1.pos.y)
	{
		int y_start = ft_max(0, ft_min(data.triangle.b.pos.y, S_HEIGHT));
		int y_end = ft_max(0, ft_min(data.triangle.c.pos.y, S_HEIGHT));
		for (int i = y_start; i < y_end; i++)
		{
			t_vertex start = vertex_add(data.triangle.b, vertex_mul_s(d1_step, (i - (int)data.triangle.b.pos.y)));
			t_vertex end = vertex_add(data.triangle.a, vertex_mul_s(d2_step, (i - (int)data.triangle.a.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);
			float tstep = 1.0f / (end.pos.x - start.pos.x);
			int x_start = ft_max(0, ft_min(start.pos.x, S_WIDTH));
			int x_end = ft_max(0, ft_min(end.pos.x, S_WIDTH));
			float t = tstep * (x_start - start.pos.x);
			for (int j = x_start; j < x_end; j++)
			{
				scanline2(ctx, data.mtl, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}
}

void	draw_triangle(t_render_context *ctx, t_render_data data)
{
	
	TexturedTriangle2(ctx, data);
	if (ctx->type == CTX_NORMAL)
	{
		uint32_t c = data.mtl->material_color_set ? data.mtl->material_color : 0xFFFFFFFF;
		t_triangle triangle = data.triangle;
		// draw_line(ctx->image, (t_pixel){triangle.a.pos.x, triangle.a.pos.y, c}, (t_pixel){triangle.b.pos.x, triangle.b.pos.y});
		// draw_line(ctx->image, (t_pixel){triangle.b.pos.x, triangle.b.pos.y, c}, (t_pixel){triangle.c.pos.x, triangle.c.pos.y});
		// draw_line(ctx->image, (t_pixel){triangle.c.pos.x, triangle.c.pos.y, c}, (t_pixel){triangle.a.pos.x, triangle.a.pos.y});
	}
}
