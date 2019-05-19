/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:17:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/19 23:20:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void	swap(t_vec4 *a, t_vec4 *b)
{
	t_vec4 t = *a;
	*a = *b;
	*b = t;
}

int		ft_max(int a, int b)
{
	return (a > b ? a : b);
}

void	draw_flat_triangle(t_doom *doom, t_triangle4d tri, t_vec4 d0, t_vec4 d1, t_vec4 edge1)
{
	const int	y_start = fmax(ceilf(tri.a.y - 0.5), 0);
	const int	y_end = fmin(ceilf(tri.c.y - 0.5), doom->screen.height - 1);
	t_vec4		edge0;
	int			y;

	edge0 = tri.a;
	edge0 = ft_vec4_add(edge0, ft_vec4_mul_s(d0, (float)y_start + 0.5 - tri.a.y));
	edge1 = ft_vec4_add(edge1, ft_vec4_mul_s(d1, (float)y_start + 0.5 - tri.a.y));
	y = y_start - 1;
	while (++y < y_end)
	{
		const int		x_start = fmax(ceilf(edge0.x - 0.5), 0);
		const int		x_end = fmin(ceilf(edge1.x - 0.5), doom->screen.width - 1);
		t_vec4			iline = edge0;
		const float		dx = edge1.x - edge0.x;
		const t_vec4	diLine = ft_vec4_div_s(ft_vec4_sub(edge1, iline), dx);
		int 			x = x_start -1;

		iline = ft_vec4_add(iline, ft_vec4_mul_s(diLine, (float)x_start + 0.5 - edge0.x)); 
		while (++x < x_end)
		{
			const float w = 1.0 / iline.w;
			const t_vec4 attr = ft_vec4_mul_s(iline, w);

			doom->screen.pixels[y * doom->screen.width + x] = get_surface_pixel(doom->textures.bricks, 
				fmin(0, fmax(iline.x * doom->textures.bricks->w, doom->textures.bricks->w - 1)), 
				fmin(0, fmax(iline.y * doom->textures.bricks->h, doom->textures.bricks->h - 1))
			);
			iline = ft_vec4_add(iline, diLine);
		}
		edge0 = ft_vec4_add(edge0, d0);
		edge1 = ft_vec4_add(edge1, d1);
	}
}

void	draw_flat_top_triangle(t_doom *doom, t_triangle4d tri)
{
	const float		delta_y = tri.c.y - tri.a.y;
	const t_vec4	d0 = ft_vec4_div_s(ft_vec4_sub(tri.c, tri.a), delta_y);
	const t_vec4	d1 = ft_vec4_div_s(ft_vec4_sub(tri.c, tri.b), delta_y);
	
	draw_flat_triangle(doom, tri, d0, d1, tri.b);
}

void	draw_flat_bottom_triangle(t_doom *doom, t_triangle4d tri)
{
	const float		delta_y = tri.c.y - tri.a.y;
	const t_vec4	d0 = ft_vec4_div_s(ft_vec4_sub(tri.b, tri.a), delta_y);
	const t_vec4	d1 = ft_vec4_div_s(ft_vec4_sub(tri.c, tri.a), delta_y);
	
	draw_flat_triangle(doom, tri, d0, d1, tri.a);
}


void	draw_triangle(t_doom *doom, t_triangle4d triangle)
{
	t_vec4	inter;

	if (triangle.b.y < triangle.a.y)
		swap(&triangle.b, &triangle.a);
	if (triangle.c.y < triangle.b.y)
		swap(&triangle.c, &triangle.b);
	if (triangle.b.y < triangle.a.y)
		swap(&triangle.b, &triangle.a);
	if (triangle.a.y == triangle.b.y)
	{
		if (triangle.b.x < triangle.a.x)
			swap(&triangle.a, &triangle.b);
		draw_flat_top_triangle(doom, triangle);
	}
	else if (triangle.b.y == triangle.c.y)
	{
		if (triangle.c.x < triangle.b.x)
			swap(&triangle.c, &triangle.b);
		draw_flat_bottom_triangle(doom, triangle);
	}
	else
	{
		inter = ft_vec4_interpolate(triangle.a, triangle.c, (triangle.b.y - triangle.a.y) / (triangle.c.y - triangle.a.y));
		if (triangle.b.x < inter.x)
		{
			draw_flat_bottom_triangle(doom, (t_triangle4d){triangle.a, triangle.b, inter});
			draw_flat_top_triangle(doom, (t_triangle4d){triangle.b, inter, triangle.c});
		}
		else
		{
			draw_flat_bottom_triangle(doom, (t_triangle4d){triangle.a, inter, triangle.b});
			draw_flat_top_triangle(doom, (t_triangle4d){inter, triangle.b, triangle.c});
		}
	}
	draw_line(&doom->screen, (t_pixel){triangle.a.x, triangle.a.y, 0xFFFFFF}, (t_pixel){triangle.b.x, triangle.b.y});
	draw_line(&doom->screen, (t_pixel){triangle.b.x, triangle.b.y, 0xFFFFFF}, (t_pixel){triangle.c.x, triangle.c.y});
	draw_line(&doom->screen, (t_pixel){triangle.c.x, triangle.c.y, 0xFFFFFF}, (t_pixel){triangle.a.x, triangle.a.y});
}