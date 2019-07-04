/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/01 16:18:42 by llelievr         ###   ########.fr       */
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

int		min3(int a, int b, int c)
{
	return ft_min(ft_min(a, b), c);
}

int		max3(int a, int b, int c)
{
	return ft_max(ft_max(a, b), c);
}

/*void	draw_flat_triangle(t_doom *doom, t_triangle tri, t_vertex d0, t_vertex d1, t_vertex edge1)
{
	const int	y_start = ft_max(ceilf(tri.a.pos.y - 0.5), 0);
	const int	y_end = ft_min(ceilf(tri.c.pos.y - 0.5), doom->screen.height - 1);
	t_vertex	edge0;
	int			y;

	edge0 = tri.a;
	edge0 = vertex_add(edge0, vertex_mul_s(d0, (float)y_start + 0.5 - tri.a.pos.y));
	edge1 = vertex_add(edge1, vertex_mul_s(d1, (float)y_start + 0.5 - tri.a.pos.y));
	y = y_start - 1;
	while (++y < y_end)
	{
		const int		x_start = ft_max(ceilf(edge0.pos.x - 0.5), 0);
		const int		x_end = ft_min(ceilf(edge1.pos.x - 0.5), doom->screen.width - 1);
		t_vec2			iline = edge0.tex;
		const float		dx = edge1.pos.x - edge0.pos.x;
		const t_vec2	diLine = ft_vec2_div_s(ft_vec2_sub(edge1.tex, iline), dx);
		const float		z = (edge1.pos.w - edge0.pos.w) / (x_end - x_start);
		int 			x = x_start - 1;

		iline = ft_vec2_add(iline, ft_vec2_mul_s(diLine, (float)x_start + 0.5 - edge0.pos.x)); 
		while (++x < x_end)
		{
			if (doom->screen.pixels[y * (int)S_WIDTH + x] != 0)
			{
				iline = ft_vec2_add(iline, diLine);
				edge0.pos.w += z;
				continue;
			}
			doom->screen.pixels[y * (int)S_WIDTH + x] = get_surface_pixel(doom->textures.bricks, 
				ft_max(0, ft_min(((iline.x / edge0.pos.w) * doom->textures.bricks->w), doom->textures.bricks->w - 1)), 
				ft_max(0, ft_min(((iline.y / edge0.pos.w) * doom->textures.bricks->h), doom->textures.bricks->h - 1))
			);
			edge0.pos.w += z;
			iline = ft_vec2_add(iline, diLine);
		}
		edge0 = vertex_add(edge0, d0);
		edge1 = vertex_add(edge1, d1);
	}
}

void	draw_flat_top_triangle(t_doom *doom, t_triangle tri)
{
	const float		delta_y = tri.c.pos.y - tri.a.pos.y;
	const t_vertex	d0 = vertex_div_s(vertex_sub(tri.c, tri.a), delta_y);
	const t_vertex	d1 = vertex_div_s(vertex_sub(tri.c, tri.b), delta_y);
	
	draw_flat_triangle(doom, tri, d0, d1, tri.b);
}

void	draw_flat_bottom_triangle(t_doom *doom, t_triangle tri)
{
	const float		delta_y = tri.c.pos.y - tri.a.pos.y;
	const t_vertex	d0 = vertex_div_s(vertex_sub(tri.b, tri.a), delta_y);
	const t_vertex	d1 = vertex_div_s(vertex_sub(tri.c, tri.a), delta_y);
	
	draw_flat_triangle(doom, tri, d0, d1, tri.a);
}
*/

/* void	draw_flat_triangle(t_doom *doom, int x1, int y1, int x2, int y2, 
			int u1, int v1, int w1,
 			int u2, int v2, int w2,
			 )
{
	int y_start = fmax(0, fmin(y1, S_HEIGHT - 1));
	int y_end = fmax(0, fmin(y2, S_HEIGHT - 1));
	for (int i = y_start; i <= y_end; i++)
	{
		int ax = x1 + (float)(i - y1) * dax_step;
		int bx = x1 + (float)(i - y1) * dbx_step;

		float tex_su = u2 + (float)(i - y2) * du1_step;
		float tex_sv = v2 + (float)(i - y2) * dv1_step;
		float tex_sw = w2 + (float)(i - y2) * dw1_step;

		float tex_eu = u1 + (float)(i - y1) * du2_step;
		float tex_ev = v1 + (float)(i - y1) * dv2_step;
		float tex_ew = w1 + (float)(i - y1) * dw2_step;

		if (ax > bx)
		{
			swapi(&ax, &bx);
			swapf(&tex_su, &tex_eu);
			swapf(&tex_sv, &tex_ev);
			swapf(&tex_sw, &tex_ew);
		}

		float tex_u = tex_su;
		float tex_v = tex_sv;
		float tex_w = tex_sw;

		float tstep = 1.0f / ((float)(bx - ax));

		int x_start = fmax(0, fmin(ax, S_WIDTH - 1));
		int x_end = fmax(0, fmin(bx, S_WIDTH - 1));
		float t = tstep * (x_start - ax);
		for (int j = x_start; j < x_end; j++)
		{
			tex_u = (1.0f - t) * tex_su + t * tex_eu;
			tex_v = (1.0f - t) * tex_sv + t * tex_ev;
			tex_w = (1.0f - t) * tex_sw + t * tex_ew;
			if (doom->screen.pixels[i * (int)S_WIDTH + j] == 0)
			{
				doom->screen.pixels[i * (int)S_WIDTH + j] = get_surface_pixel(doom->textures.bricks, 
					ft_max(0, ft_min(((tex_u / tex_w) * doom->textures.bricks->w), doom->textures.bricks->w - 1)), 
					ft_max(0, ft_min(((tex_v / tex_w) * doom->textures.bricks->h), doom->textures.bricks->h - 1))
				);
			}
			t += tstep;
		}
	}
}
 */
void TexturedTriangle(t_doom *doom,	int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3)
{
	if (y2 < y1)
	{
		swapi(&y1, &y2);
		swapi(&x1, &x2);
		swapf(&u1, &u2);
		swapf(&v1, &v2);
		swapf(&w1, &w2);
	}

	if (y3 < y1)
	{
		swapi(&y1, &y3);
		swapi(&x1, &x3);
		swapf(&u1, &u3);
		swapf(&v1, &v3);
		swapf(&w1, &w3);
	}

	if (y3 < y2)
	{
		swapi(&y2, &y3);
		swapi(&x2, &x3);
		swapf(&u2, &u3);
		swapf(&v2, &v3);
		swapf(&w2, &w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step=0, dw2_step=0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		int y_start = fmax(0, fmin(y1, S_HEIGHT - 1));
		int y_end = fmax(0, fmin(y2, S_HEIGHT - 1));
		for (int i = y_start; i <= y_end; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swapi(&ax, &bx);
				swapf(&tex_su, &tex_eu);
				swapf(&tex_sv, &tex_ev);
				swapf(&tex_sw, &tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));

			int x_start = fmax(0, fmin(ax, S_WIDTH - 1));
			int x_end = fmax(0, fmin(bx, S_WIDTH - 1));
			float t = tstep * (x_start - ax);
			for (int j = x_start; j < x_end; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (doom->screen.pixels[i * (int)S_WIDTH + j] == 0)
				{
					doom->screen.pixels[i * (int)S_WIDTH + j] = get_surface_pixel(doom->textures.bricks, 
						ft_max(0, ft_min(((tex_u / tex_w) * doom->textures.bricks->w), doom->textures.bricks->w - 1)), 
						ft_max(0, ft_min(((tex_v / tex_w) * doom->textures.bricks->h), doom->textures.bricks->h - 1))
					);
				}
				t += tstep;
			}
		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		int y_start = fmax(0, fmin(y2, S_HEIGHT - 1));
		int y_end = fmax(0, fmin(y3, S_HEIGHT - 1));
		for (int i = y_start; i <= y_end; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swapi(&ax, &bx);
				swapf(&tex_su, &tex_eu);
				swapf(&tex_sv, &tex_ev);
				swapf(&tex_sw, &tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));

			int x_start = fmax(0, fmin(ax, S_WIDTH - 1));
			int x_end = fmax(0, fmin(bx, S_WIDTH - 1));
			float t = tstep * (x_start - ax);
			for (int j = x_start; j < x_end; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;

				if (doom->screen.pixels[i * (int)S_WIDTH + j] == 0)
				{
					doom->screen.pixels[i * (int)S_WIDTH + j] = get_surface_pixel(doom->textures.bricks, 
						ft_max(0, ft_min(((tex_u / tex_w) * doom->textures.bricks->w), doom->textures.bricks->w - 1)), 
						ft_max(0, ft_min(((tex_v / tex_w) * doom->textures.bricks->h), doom->textures.bricks->h - 1))
					);
				}
				t += tstep;
			}
		}
	}
}

void	draw_triangle(t_doom *doom, t_triangle triangle)
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
	TexturedTriangle(doom, triangle.a.pos.x, triangle.a.pos.y, triangle.a.tex.x, triangle.a.tex.y, triangle.a.pos.w,
	triangle.b.pos.x, triangle.b.pos.y, triangle.b.tex.x, triangle.b.tex.y, triangle.b.pos.w,
	triangle.c.pos.x, triangle.c.pos.y, triangle.c.tex.x, triangle.c.tex.y, triangle.c.pos.w);
/*	draw_line(&doom->screen, (t_pixel){triangle.a.pos.x, triangle.a.pos.y, 0xFFFFFF}, (t_pixel){triangle.b.pos.x, triangle.b.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.b.pos.x, triangle.b.pos.y, 0xFFFFFF}, (t_pixel){triangle.c.pos.x, triangle.c.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.c.pos.x, triangle.c.pos.y, 0xFFFFFF}, (t_pixel){triangle.a.pos.x, triangle.a.pos.y});*/
}
