/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/19 17:56:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <pthread.h>

static inline void	swap(t_vertex *a, t_vertex *b)
{
	t_vertex t = *a;
	*a = *b;
	*b = t;
}

static void	swapi(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

static void	swapf(float *a, float *b)
{
	float t = *a;
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

void scanline2(t_doom *doom, t_mtl *mtl, t_pixel p, float t, t_vertex start, t_vertex end)
{
	t_vertex	vert;

	vert.pos.w = (1.0f - t) * start.pos.w + t * end.pos.w;
	if (vert.pos.w <= doom->rendered_area[p.y * (int)S_WIDTH + p.x])
	{
		doom->rendered_area[p.y * (int)S_WIDTH + p.x] = vert.pos.w;
		if (mtl->texture_map_set)
		{
			vert.tex.x = (1.0f - t) * start.tex.x + t * end.tex.x;
			vert.tex.y = (1.0f - t) * start.tex.y + t * end.tex.y;
			vert.pos.z = (1.0f - t) * start.pos.z + t * end.pos.z;
			vert.normal = ft_vec3_add(start.normal, ft_vec3_mul_s(ft_vec3_sub(end.normal, start.normal), t));
			// if (vert.pos.z >= (FAR_CULL - 5))
			// {
			// 	float w = 1. / vert.pos.w;
			// 	ur_color c;
			// 	c.color = get_surface_pixel(mtl->texture_map,
			// 		fmax(0, fmin(mtl->texture_map->w - 1, (vert.tex.x * w) * (mtl->texture_map->w - 1))),
			// 		fmax(0, fmin(mtl->texture_map->h - 1, (1. - (vert.tex.y * w)) * (mtl->texture_map->h - 1)))
			// 	);
			// 	float factor = 1 - (FAR_CULL - vert.pos.z) / (FAR_CULL - (FAR_CULL - 5));
			// 	c.color = ft_color_i(ft_color_gradient((t_color){ c.argb.r, c.argb.g, c.argb.b, 255}, (t_color){ 0, 0, 255, 255}, fmax(0, fmin(1, factor))));
			// 	doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = c.color;
			// }
			// else
			{
				float w = 1. / vert.pos.w;
			 	 doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = get_surface_pixel(mtl->texture_map,
					ft_max(0, ft_min(mtl->texture_map->w - 1, (vert.tex.x * w) * (mtl->texture_map->w - 1))),
					ft_max(0, ft_min(mtl->texture_map->h - 1, (1. - (vert.tex.y * w)) * (mtl->texture_map->h - 1)))
				);
				
				// ur_color c;
				// // c.color = 0xFFFF00FF;
				// c.color = get_surface_pixel(mtl->texture_map,
				// 	ft_max(0, ft_min(mtl->texture_map->w - 1, (vert.tex.x * w) * (mtl->texture_map->w - 1))),
				// 	ft_max(0, ft_min(mtl->texture_map->h - 1, (1. - (vert.tex.y * w)) * (mtl->texture_map->h - 1)))
				// );
				// t_vec3 d = ft_vec3_mul(ft_vec3_inv(vert.normal), (t_vec3){1, 0, 1});
				// t_vec3 ambiant = (t_vec3){0.5, 0.5, 0.5};
				// t_vec3 v = ft_vec3_add(ambiant, d);
				// //float f = 1. / 255.;
				// c.argb.r = ((float)c.argb.r + v.x * 255)/2;
				// c.argb.g = ((float)c.argb.g + v.y * 255)/2;
				// c.argb.b = ((float)c.argb.b + v.z * 255)/2;
				// doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = c.color;
			}
		}
		else if (mtl->material_color_set)
		{
// 			t_color c = ft_i_color(mtl->material_color);
// 			t_vec3 d = ft_vec3_mul_s((t_vec3){1, 1, 1}, fmax(0, ft_vec3_dot(ft_vec3_inv(vert.normal), (t_vec3){0, 0, 1})));
// //			
				
// 			t_vec3 ambiant = (t_vec3){0.1, 0.1, 0.1};
// 			t_vec3 v = ft_vec3_add(ambiant,d);
// //			
// 			c.r = fmin(1, (c.r / 255.) * v.x) * 255;
// 			c.g = fmin(1, (c.g / 255.) * v.y) * 255;
// 			c.b = fmin(1, (c.b / 255.) * v.z) * 255;
			doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = (uint32_t)mtl->material_color;
		}
	}
}

typedef struct s_data
{
	int			i;
	t_vertex	start;
	t_vertex	end;
	t_doom		*doom;
	t_mtl		*mtl;
}				t_data;

void TexturedTriangle2(t_doom *doom, t_vertex v1, t_vertex v2, t_vertex v3, t_mtl *mtl)
{
	if (v2.pos.y < v1.pos.y)
		swap(&v1, &v2);
	if (v3.pos.y < v1.pos.y)
		swap(&v1, &v3);
	if (v3.pos.y < v2.pos.y)
		swap(&v2, &v3);
	t_vertex	d1 = vertex_sub(v2, v1);
	t_vertex	d2 = vertex_sub(v3, v1);
	t_vertex	d1_step;
	t_vertex	d2_step;

	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d2.pos.y)
		d2_step = vertex_div_s(d2, fabsf(d2.pos.y));
	if (d1.pos.y)
	{
		int y_start = ft_max(0, ft_min(v1.pos.y, S_HEIGHT - 1));
		int y_end = ft_max(0, ft_min(v2.pos.y, S_HEIGHT - 1));
		
		int t = 0;
		//pthread_t threads[y_end];
		for (int i = y_start; i < y_end; i++)
		{
			t_vertex start = vertex_add(v1, vertex_mul_s(d1_step, (i - (int)v1.pos.y)));
			t_vertex end = vertex_add(v1, vertex_mul_s(d2_step, (i - (int)v1.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);
			float tstep = 1.0f / (end.pos.x - start.pos.x);
			int x_start = ft_max(0, ft_min(start.pos.x, S_WIDTH - 1));
			int x_end = ft_max(0, ft_min(end.pos.x, S_WIDTH - 1));
			float t = tstep * (x_start - start.pos.x);
			for (int j = x_start; j < x_end; j++)
			{
				scanline2(doom, mtl, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
			/* t_data data = (t_data){
				.i = i,
				.mtl = mtl,
				.start = start,
				.end = end,
				.doom = doom
			}; */
			//	d_line(&data);
		}
		/* t = 0;
		for (int i = y_start; i < y_end; i++)
		{
			if (pthread_join(threads[t++], NULL) != 0)
				continue;
		} */
	}

	d1 = vertex_sub(v3, v2);
	if (d1.pos.y)
		d1_step = vertex_div_s(d1, fabsf(d1.pos.y));
	if (d1.pos.y)
	{
		int y_start = ft_max(0, ft_min(v2.pos.y, S_HEIGHT - 1));
		int y_end = ft_max(0, ft_min(v3.pos.y, S_HEIGHT - 1));
		for (int i = y_start; i < y_end; i++)
		{
			t_vertex start = vertex_add(v2, vertex_mul_s(d1_step, (i - (int)v2.pos.y)));
			t_vertex end = vertex_add(v1, vertex_mul_s(d2_step, (i - (int)v1.pos.y)));
			if (start.pos.x > end.pos.x)
				swap(&start, &end);
			float tstep = 1.0f / (end.pos.x - start.pos.x);
			int x_start = ft_max(0, ft_min(start.pos.x, S_WIDTH - 1));
			int x_end = ft_max(0, ft_min(end.pos.x, S_WIDTH - 1));
			float t = tstep * (x_start - start.pos.x);
			for (int j = x_start; j < x_end; j++)
			{
				scanline2(doom, mtl, (t_pixel){ j, i, 0 }, t, start, end);
				t += tstep;
			}
		}
	}
}

void	draw_triangle(t_doom *doom, t_triangle triangle, t_mtl *mtl)
{
	
	TexturedTriangle2(doom, triangle.a, triangle.b, triangle.c, mtl);
	uint32_t c = mtl->material_color_set ? mtl->material_color : 0xFFFFFF;
 	draw_line(&doom->screen, (t_pixel){triangle.a.pos.x, triangle.a.pos.y, c}, (t_pixel){triangle.b.pos.x, triangle.b.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.b.pos.x, triangle.b.pos.y, c}, (t_pixel){triangle.c.pos.x, triangle.c.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.c.pos.x, triangle.c.pos.y, c}, (t_pixel){triangle.a.pos.x, triangle.a.pos.y});
}
