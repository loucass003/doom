/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 15:42:57 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths/vertex.h"
#include "doom.h"
#include "render.h"
#include "maths/triangle.h"
#include "image.h"
#include <pthread.h>
#include <stdatomic.h>

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
} ur_color;

void scanline2(t_render_context *ctx, t_triangle triangle, t_pixel p, float t, t_vertex start, t_vertex end)
{
	t_vertex	vert;
	float		*buff;
	

	vert.pos.w = (1.0f - t) * start.pos.w + t * end.pos.w;
	buff = ctx->buffer + p.y * (int)S_WIDTH + p.x;
	// if (vert.pos.w > -25)
	// 	return ;
	t_bool render = FALSE;
//	pthread_mutex_lock(&ctx->doom->mutex);

	//pthread_mutex_unlock(&ctx->doom->mutex);
	if (vert.pos.w <= *buff)
	{
		// vert.normal.x = (1.0f - t) * start.normal.x + t * end.normal.x;
		// vert.normal.y = (1.0f - t) * start.normal.y + t * end.normal.y;
		// vert.normal.z = (1.0f - t) * start.normal.z + t * end.normal.z;
		// vert.pos.x =  (1.0f - t) * start.pos.x + t * end.pos.x;
		// vert.pos.y =  (1.0f - t) * start.pos.y + t * end.pos.y;
		// vert.pos.z =  (1.0f - t) * start.pos.z + t * end.pos.z;
		float lt_color = (1.0f - t) * start.light_color + t * end.light_color;
		 ur_color c;
		float a = ft_max(AMBIANT_LIGHT, lt_color) / 255.0;
		t_face		*face = &triangle.r->faces->values[triangle.face_index];
		t_mtl		*mtl = &triangle.r->materials->values[face->mtl_index];
		if (mtl->texture_map_set)
		{
			float w = 1. / vert.pos.w;
			vert.tex.x = (1.0f - t) * start.tex.x + t * end.tex.x ;
			vert.tex.y = ((1.0f - t) * start.tex.y + t * end.tex.y);
			
			// int x = (int)(ft_abs(((vert.tex.x > 0) - vert.tex.x * w) * (mtl->texture_map->width))) % (mtl->texture_map->width);
			// int y = (int)(ft_abs(((vert.tex.y > 0) - vert.tex.y * w) * (mtl->texture_map->height))) % (mtl->texture_map->height);

			// x = vert.tex.x > 0 ? (mtl->texture_map->width - 1) - x : x;
			// y = vert.tex.y < 0 ? (mtl->texture_map->height - 1) - y : y;
			
			int x = fabs((int)((vert.tex.x * w) * mtl->texture_map->width) % mtl->texture_map->width);
			int y = (mtl->texture_map->height - 1) - fabs((int)((vert.tex.y * w) * mtl->texture_map->height) % mtl->texture_map->height);
			if (face->lightmap.map)
			{
				// int t_w = mtl->texture_map->width;
				// int t_h = mtl->texture_map->height;

				// t_vec2 uv;
				// int x0 = (int)(ft_abs(((vert.tex_lm.x > 0) - vert.tex_lm.x * w) * (mtl->texture_map->width))) % (mtl->texture_map->width);
				// int y0 = (int)(ft_abs(((vert.tex_lm.y > 0) - vert.tex_lm.y * w) * (mtl->texture_map->height))) % (mtl->texture_map->height);

				// x0 = vert.tex_lm.x > 0 ? (mtl->texture_map->width - 1) - x0 : x0;
				// y0 = vert.tex_lm.y < 0 ? (mtl->texture_map->height - 1) - y0 : y0;
				a = face->lightmap.map[(y * mtl->texture_map->width) + x] / 255.0;
			}
			c.color = mtl->texture_map->pixels[(y * mtl->texture_map->width) + x];
			if (c.argb.a == 0)
				return ;
		}
		else if (mtl->material_color_set)
			c.color = mtl->material_color;
		if (!mtl->transparent)
		{
			c.argb.r *= a;
			c.argb.g *= a;
			c.argb.b *= a;
		}	
		// c.argb.a = 255;
		// c.argb.r = vert.normal.x * 255;
		// c.argb.g = vert.normal.y * 255;
		// c.argb.b = vert.normal.z * 255;
		ctx->image->pixels[p.y * (int)S_WIDTH + p.x] = c.color;
		*buff = vert.pos.w;
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

void TexturedTriangle2(t_render_context *ctx, t_triangle triangle)
{
	if (triangle.b.pos.y < triangle.a.pos.y)
		swap(&triangle.a, &triangle.b);
	if (triangle.c.pos.y < triangle.a.pos.y)
		swap(&triangle.a, &triangle.c);
	if (triangle.c.pos.y < triangle.b.pos.y)
		swap(&triangle.b, &triangle.c);
	t_vertex	d1 = vertex_sub(triangle.b, triangle.a);
	t_vertex	d2 = vertex_sub(triangle.c, triangle.a);
	t_vertex	d1_step;
	t_vertex	d2_step;


	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d2.pos.y)
		d2_step = vertex_div_s(d2, fabsf(d2.pos.y));
	if (d1.pos.y)
	{
		int y_start = ft_max(0, ft_min(triangle.a.pos.y, S_HEIGHT));
		int y_end = ft_max(0, ft_min(triangle.b.pos.y, S_HEIGHT));
		
		for (int i = y_start; i < y_end; i++)
		{
			t_vertex start = vertex_add(triangle.a, vertex_mul_s(d1_step, (i - (int)triangle.a.pos.y)));
			t_vertex end = vertex_add(triangle.a, vertex_mul_s(d2_step, (i - (int)triangle.a.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);
			float tstep = 1.0f / (end.pos.x - start.pos.x);
			int x_start = ft_max(0, ft_min(start.pos.x, S_WIDTH));
			int x_end = ft_max(0, ft_min(end.pos.x, S_WIDTH));
			float t = tstep * (x_start - start.pos.x);
			for (int j = x_start; j < x_end; j++)
			{
				scanline2(ctx, triangle, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}

	d1 = vertex_sub(triangle.c, triangle.b);
	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d1.pos.y)
	{
		int y_start = ft_max(0, ft_min(triangle.b.pos.y, S_HEIGHT));
		int y_end = ft_max(0, ft_min(triangle.c.pos.y, S_HEIGHT));
		
		for (int i = y_start; i < y_end; i++)
		{
			t_vertex start = vertex_add(triangle.b, vertex_mul_s(d1_step, (i - (int)triangle.b.pos.y)));
			t_vertex end = vertex_add(triangle.a, vertex_mul_s(d2_step, (i - (int)triangle.a.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);
			float tstep = 1.0f / (end.pos.x - start.pos.x);
			int x_start = ft_max(0, ft_min(start.pos.x, S_WIDTH));
			int x_end = ft_max(0, ft_min(end.pos.x, S_WIDTH));
			float t = tstep * (x_start - start.pos.x);
			for (int j = x_start; j < x_end; j++)
			{
				scanline2(ctx, triangle, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}
}

void	draw_triangle(t_render_context *ctx, t_triangle triangle)
{
	t_face		*face = &triangle.r->faces->values[triangle.face_index];
	t_mtl		*mtl = &triangle.r->materials->values[face->mtl_index];
	
	if (!mtl->wireframe)
		TexturedTriangle2(ctx, triangle);
	if (mtl->wireframe)
	{
		uint32_t c = mtl->material_color_set ? mtl->material_color : 0xFFFFFFFF;
		// if (c != 0xFFFF0000)
		// 	return ;
		draw_line_zbuff(ctx, triangle.a.pos, triangle.b.pos, c);
		draw_line_zbuff(ctx, triangle.b.pos, triangle.c.pos, c);
		draw_line_zbuff(ctx, triangle.c.pos, triangle.a.pos, c);
	}
}
