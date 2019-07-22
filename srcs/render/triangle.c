/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/21 21:21:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void	swap(t_vertex *a, t_vertex *b)
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

void scanline(t_doom *doom, t_mtl *mtl, t_pixel p, float t, t_vec4 tex_s, t_vec4 tex_e, t_vec3 n1, t_vec3 n2, t_vec3 n3)
{
	t_vec4		uvw;

	uvw.w = (1.0f - t) * tex_s.w + t * tex_e.w;
	if (uvw.w <= doom->rendered_area[p.y * (int)S_WIDTH + p.x])
	{
		doom->rendered_area[p.y * (int)S_WIDTH + p.x] = uvw.w;
		if (mtl->texture_map_set)
		{
			uvw.x = (1.0f - t) * tex_s.x + t * tex_e.x;
			uvw.y = (1.0f - t) * tex_s.y + t * tex_e.y;
			uvw.z = (1.0f - t) * tex_s.z + t * tex_e.z;
			if (uvw.z >= (FAR_CULL - 5))
			{
				t_color c = ft_i_color(get_surface_pixel(mtl->texture_map, 
					((uvw.x / uvw.w) * (mtl->texture_map->w)), 
					((1 - (uvw.y / uvw.w)) * (mtl->texture_map->h))
				));
				float factor = 1 - (FAR_CULL - uvw.z) / (FAR_CULL - (FAR_CULL - 5));
				c = ft_color_gradient(c, (t_color){ 0, 0, 255 }, fmax(0, fmin(1, factor)));
				doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = (uint32_t)ft_color_i(c);
			}
			else
			{
				doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = get_surface_pixel(mtl->texture_map, 
					((uvw.x / uvw.w) * (mtl->texture_map->w)), 
					((1 - (uvw.y / uvw.w)) * (mtl->texture_map->h))
				);
			/* 	t_color c = ft_i_color(get_surface_pixel(mtl->texture_map, 
					((uvw.x / uvw.w) * (mtl->texture_map->w)), 
					((1 - (uvw.y / uvw.w)) * (mtl->texture_map->h))
				));
				float it = ft_vec3_dot(n1, (t_vec3){0, 0, -1});
			//	printf("%f\n", it);
				c = ft_color_gradient(c, (t_color){ 0, 0, 0 }, fmax(0, fmin(0.5, it)));
				doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = (uint32_t)ft_color_i(c); */
			}
		}
		else if (mtl->material_color_set)
			doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = mtl->material_color;
	}
}

void TexturedTriangle(t_doom *doom,	int x1, int y1, float u1, float v1, float w1, float z1, t_vec3 n1,
							int x2, int y2, float u2, float v2, float w2, float z2, t_vec3 n2,
							int x3, int y3, float u3, float v3, float w3, float z3, t_vec3 n3, t_mtl *mtl)
{
	if (y2 < y1)
	{
		swapi(&y1, &y2);
		swapi(&x1, &x2);
		swapi(&z1, &z2);
		swapf(&u1, &u2);
		swapf(&v1, &v2);
		swapf(&w1, &w2);
	}

	if (y3 < y1)
	{
		swapi(&y1, &y3);
		swapi(&x1, &x3);
		swapi(&z1, &z3);
		swapf(&u1, &u3);
		swapf(&v1, &v3);
		swapf(&w1, &w3);
	}

	if (y3 < y2)
	{
		swapi(&y2, &y3);
		swapi(&x2, &x3);
		swapi(&z2, &z3);
		swapf(&u2, &u3);
		swapf(&v2, &v3);
		swapf(&w2, &w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dz1 = z2 - z1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dz2 = z3 - z1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w, tex_z;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step=0, dw2_step=0, dz1_step = 0, dz2_step = 0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);
	if (dy1) dz1_step = dz1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);
	if (dy2) dz2_step = dz2 / (float)abs(dy2);

	if (dy1)
	{
		int y_start = fmax(0, fmin(y1, S_HEIGHT - 1));
		int y_end = fmax(0, fmin(y2, S_HEIGHT - 1));
		for (int i = y_start; i < y_end; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;
			float tex_sz = z1 + (float)(i - y1) * dz1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;
			float tex_ez = z1 + (float)(i - y1) * dz2_step;


			if (ax > bx)
			{
				swapi(&ax, &bx);
				swapf(&tex_su, &tex_eu);
				swapf(&tex_sv, &tex_ev);
				swapf(&tex_sw, &tex_ew);
				swapf(&tex_sz, &tex_ez);
			}

			float tstep = 1.0f / ((float)(bx - ax));

			int x_start = fmax(0, fmin(ax, S_WIDTH - 1));
			int x_end = fmax(0, fmin(bx, S_WIDTH - 1));
			float t = tstep * (x_start - ax);
			for (int j = x_start; j < x_end; j++)
			{
				scanline(doom, mtl, (t_pixel){ j, i, 0 }, t, (t_vec4){ tex_su, tex_sv, tex_sz, tex_sw }, (t_vec4){ tex_eu, tex_ev, tex_ez, tex_ew }, n1, n2, n3);
				t += tstep;
			}
		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;
	dz1 = z3 - z2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);
	if (dy1) dz1_step = dz1 / (float)abs(dy1);

	if (dy1)
	{
		int y_start = fmax(0, fmin(y2, S_HEIGHT - 1));
		int y_end = fmax(0, fmin(y3, S_HEIGHT - 1));
		for (int i = y_start; i < y_end; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;
			float tex_sz = z2 + (float)(i - y2) * dz1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;
			float tex_ez = z1 + (float)(i - y1) * dz2_step;

			if (ax > bx)
			{
				swapi(&ax, &bx);
				swapf(&tex_su, &tex_eu);
				swapf(&tex_sv, &tex_ev);
				swapf(&tex_sw, &tex_ew);
				swapf(&tex_sz, &tex_ez);
			}

			float tstep = 1.0f / ((float)(bx - ax));

			int x_start = fmax(0, fmin(ax, S_WIDTH - 1));
			int x_end = fmax(0, fmin(bx, S_WIDTH - 1));
			float t = tstep * (x_start - ax);
			for (int j = x_start; j < x_end; j++)
			{
				scanline(doom, mtl, (t_pixel){ j, i, 0 }, t, (t_vec4){ tex_su, tex_sv, tex_sz, tex_sw }, (t_vec4){ tex_eu, tex_ev, tex_ez, tex_ew }, n1, n2, n3);
				t += tstep;
			}
		}
	}
}

void	draw_triangle(t_doom *doom, t_triangle triangle, t_mtl *mtl)
{
	/*t_vertex	inter;

	if (triangle.b.pos.y < triangle.a.pos.y)
		swap(&triangle.b, &triangle.a);
	if (triangle.c.pos.y < triangle.b.pos.y)
		swap(&triangle.c, &triangle.b);
	if (triangle.b.pos.y < triangle.a.pos.y)
		swap(&triangle.b, &triangle.a);
	if (triangle.a.pos.y == triangle.b.pos.y)
	{
		if (triangle.b.pos.x < triangle.a.pos.x)
			swap(&triangle.a, &triangle.b);
		draw_flat_top_triangle(doom, triangle);
	}
	else if (triangle.b.pos.y == triangle.c.pos.y)
	{
		if (triangle.c.pos.x < triangle.b.pos.x)
			swap(&triangle.c, &triangle.b);
		draw_flat_bottom_triangle(doom, triangle);
	}
	else
	{
		inter = vertex_interpolate(triangle.a, triangle.c, (triangle.b.pos.y - triangle.a.pos.y) / (triangle.c.pos.y - triangle.a.pos.y));
		if (triangle.b.pos.x < inter.pos.x)
		{
			draw_flat_bottom_triangle(doom, (t_triangle){triangle.a, triangle.b, inter});
			draw_flat_top_triangle(doom, (t_triangle){triangle.b, inter, triangle.c});
		}
		else
		{
			draw_flat_bottom_triangle(doom, (t_triangle){triangle.a, inter, triangle.b});
			draw_flat_top_triangle(doom, (t_triangle){inter, triangle.b, triangle.c});
		}
	}*/
	 /*TexturedTriangle(t_doom *doom,	int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3)*/
	TexturedTriangle(doom, triangle.a.pos.x, triangle.a.pos.y, triangle.a.tex.x, triangle.a.tex.y, triangle.a.pos.w, triangle.a.pos.z, triangle.a.normal,
	triangle.b.pos.x, triangle.b.pos.y, triangle.b.tex.x, triangle.b.tex.y, triangle.b.pos.w, triangle.b.pos.z, triangle.b.normal,
	triangle.c.pos.x, triangle.c.pos.y, triangle.c.tex.x, triangle.c.tex.y, triangle.c.pos.w, triangle.c.pos.z, triangle.c.normal, mtl);
 	/* draw_line(&doom->screen, (t_pixel){triangle.a.pos.x, triangle.a.pos.y, 0xFFFFFF}, (t_pixel){triangle.b.pos.x, triangle.b.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.b.pos.x, triangle.b.pos.y, 0xFFFFFF}, (t_pixel){triangle.c.pos.x, triangle.c.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.c.pos.x, triangle.c.pos.y, 0xFFFFFF}, (t_pixel){triangle.a.pos.x, triangle.a.pos.y});  */
}
