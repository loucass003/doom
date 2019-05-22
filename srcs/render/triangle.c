/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/22 23:07:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void	swap(t_vertex *a, t_vertex *b)
{
	t_vertex t = *a;
	*a = *b;
	*b = t;
}

int		ft_max(int a, int b)
{
	return (a > b ? a : b);
}

void	draw_flat_triangle(t_doom *doom, t_triangle3d tri, t_vertex d0, t_vertex d1, t_vertex edge1)
{
	const int	y_start = fmax(ceilf(tri.a.pos.y - 0.5), 0);
	const int	y_end = fmin(ceilf(tri.c.pos.y - 0.5), doom->screen.height - 1);
	t_vertex	edge0;
	int			y;

	edge0 = tri.a;
	edge0 = vertex_add(edge0, vertex_mul_s(d0, (float)y_start + 0.5 - tri.a.pos.y));
	edge1 = vertex_add(edge1, vertex_mul_s(d1, (float)y_start + 0.5 - tri.a.pos.y));
	y = y_start - 1;
	while (++y < y_end)
	{
		const int		x_start = fmax(ceilf(edge0.pos.x - 0.5), 0);
		const int		x_end = fmin(ceilf(edge1.pos.x - 0.5), doom->screen.width - 1);
		t_vec2			iline = edge0.tex;
		const float		dx = edge1.pos.x - edge0.pos.x;
		const t_vec2	diLine = ft_vec2_div_s(ft_vec2_sub(edge1.tex, iline), dx);
		int 			x = x_start - 1;

		iline = ft_vec2_add(iline, ft_vec2_mul_s(diLine, (float)x_start + 0.5 - edge0.pos.x)); 
		while (++x < x_end)
		{
			doom->screen.pixels[y * (int)S_WIDTH + x] = get_surface_pixel(doom->textures.bricks, 
				fmax(0, fmin(iline.x * doom->textures.bricks->w, doom->textures.bricks->w - 1)), 
				fmax(0, fmin(iline.y * doom->textures.bricks->h, doom->textures.bricks->h - 1))
			);
			iline = ft_vec2_add(iline, diLine);
		}
		edge0 = vertex_add(edge0, d0);
		edge1 = vertex_add(edge1, d1);
	}
}

void	draw_flat_top_triangle(t_doom *doom, t_triangle3d tri)
{
	const float		delta_y = tri.c.pos.y - tri.a.pos.y;
	const t_vertex	d0 = vertex_div_s(vertex_sub(tri.c, tri.a), delta_y);
	const t_vertex	d1 = vertex_div_s(vertex_sub(tri.c, tri.b), delta_y);
	
	draw_flat_triangle(doom, tri, d0, d1, tri.b);
}

void	draw_flat_bottom_triangle(t_doom *doom, t_triangle3d tri)
{
	const float		delta_y = tri.c.pos.y - tri.a.pos.y;
	const t_vertex	d0 = vertex_div_s(vertex_sub(tri.b, tri.a), delta_y);
	const t_vertex	d1 = vertex_div_s(vertex_sub(tri.c, tri.a), delta_y);
	
	draw_flat_triangle(doom, tri, d0, d1, tri.a);
}


void	draw_triangle(t_doom *doom, t_triangle3d triangle)
{
	t_vertex	inter;

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
			draw_flat_bottom_triangle(doom, (t_triangle3d){triangle.a, triangle.b, inter});
			draw_flat_top_triangle(doom, (t_triangle3d){triangle.b, inter, triangle.c});
		}
		else
		{
			draw_flat_bottom_triangle(doom, (t_triangle3d){triangle.a, inter, triangle.b});
			draw_flat_top_triangle(doom, (t_triangle3d){inter, triangle.b, triangle.c});
		}
	}
	draw_line(&doom->screen, (t_pixel){triangle.a.pos.x, triangle.a.pos.y, 0xFFFFFF}, (t_pixel){triangle.b.pos.x, triangle.b.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.b.pos.x, triangle.b.pos.y, 0xFFFFFF}, (t_pixel){triangle.c.pos.x, triangle.c.pos.y});
	draw_line(&doom->screen, (t_pixel){triangle.c.pos.x, triangle.c.pos.y, 0xFFFFFF}, (t_pixel){triangle.a.pos.x, triangle.a.pos.y});
}
