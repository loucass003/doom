/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/15 00:53:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#define NEAR (1e-1)

t_bool	prepare_polygon(t_polygon *poly)
{
	if (!poly->proj_vertices 
		&& !(poly->proj_vertices = create_2dvertices_array(
				poly->vertices->len * 2)))
		return (FALSE); //TODO: ERROR
	if (!poly->proj_vertices_buf 
		&& !(poly->proj_vertices_buf = create_2dvertices_array(
				poly->proj_vertices->capacity)))
		return (FALSE);
	if (poly->proj_vertices)
		poly->proj_vertices->len = 0;
	if (poly->proj_vertices_buf)
		poly->proj_vertices_buf->len = 0;
	return (TRUE);
}

int	max3(int a, int b, int c)
{
	return (fmax(fmax(a, b), c));
}

int	min3(int a, int b, int c)
{
	return (fmin(fmin(a, b), c));
}

int	ft_pixel_dot(t_pixel a, t_pixel b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

t_bool	inside_triangle2d(t_pixel a, t_pixel b, t_pixel c, t_pixel p, float *u, float *v)
{
	t_pixel v0 = ft_pixel_sub(c, a);
	t_pixel v1 = ft_pixel_sub(b, a);
	t_pixel v2 = ft_pixel_sub(p, a);

	// Compute dot products
	float dot00 = ft_pixel_dot(v0, v0);
	float dot01 = ft_pixel_dot(v0, v1);
	float dot02 = ft_pixel_dot(v0, v2);
	float dot11 = ft_pixel_dot(v1, v1);
	float dot12 = ft_pixel_dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1. / (dot00 * dot11 - dot01 * dot01);
	*u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	*v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (*u >= 0) && (*v >= 0) && ((*u) + (*v) < 1);
}

void	fill_triangle(t_doom *doom, int color, t_pixel v1, t_pixel v2, t_pixel v3)
{
	t_pixel max = (t_pixel){fmin(max3(v1.x, v2.x, v3.x), S_WIDTH - 1), fmin(max3(v1.y, v2.y, v3.y), S_HEIGHT - 1)};
	t_pixel min = (t_pixel){fmax(min3(v1.x, v2.x, v3.x), 0), fmax(min3(v1.y, v2.y, v3.y), 0)};
	t_pixel p = (t_pixel){ min.x - 1, min.y - 1, 0};

	while (++p.y < max.y && (p.x = min.x - 1))
	{
		while (++p.x < max.x)
		{
			float u,v;
			if (inside_triangle2d(v1, v2, v3, p, &u, &v))
			{
				doom->screen.pixels[p.y * (int)S_WIDTH + p.x] = color;
			}
		}
	}
}

void	render_polygon(t_doom *doom, t_polygon *poly)
{
		//fill_triangle(doom, 0xFF, (t_vec2){ 10, 100 }, (t_vec2){ 100, 50 }, (t_vec2){ 5, 200 });

	t_vec2	last_b = (t_vec2){ NAN, NAN };

	if (poly->vertices->len < 3)
		return;
	if (!prepare_polygon(poly))
		return ; // TODO: ERROR
	float f = ft_vec3_dot(get_polygon_normal(poly), ft_vec3_sub(doom->player.pos, poly->vertices->vertices[0]));
	if (f <= 0)
		return;
	int i = 0;
	//printf("%d\n", poly->indices->len);
	while (i < poly->indices->len)
	{
		//int next = (i + 1) % poly->indices->len;
		//printf("%d\n", poly->indices->values[i]);
		t_vec3 a = ft_mat4_mulv(doom->player.matrix, poly->vertices->vertices[poly->indices->values[i]]);
		t_vec3 b = ft_mat4_mulv(doom->player.matrix, poly->vertices->vertices[poly->indices->values[i + 1]]);
		t_vec3 c = ft_mat4_mulv(doom->player.matrix, poly->vertices->vertices[poly->indices->values[i + 2]]);

		if (a.z < NEAR || b.z < NEAR || c.z < NEAR)
		{
			i += 3;
			continue;
		}

		a = ft_mat4_mulv(doom->player.projection, a);
		b = ft_mat4_mulv(doom->player.projection, b);
		c = ft_mat4_mulv(doom->player.projection, c);

		t_pixel p = (t_pixel){
			((a.x + 0.5) / a.z) * S_WIDTH,
			S_HEIGHT_2 + (a.y / a.z) * S_HEIGHT + (poly->type == P_FLOOR),
		//	poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000
		};
		t_pixel p2 = (t_pixel){
			((b.x + 0.5) / b.z) * S_WIDTH,
			S_HEIGHT_2 + (b.y / b.z) * S_HEIGHT + (poly->type == P_FLOOR)
		//	poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000
		};
		t_pixel p3 = (t_pixel){
			((c.x + 0.5) / c.z) * S_WIDTH,
			S_HEIGHT_2 + (c.y / c.z) * S_HEIGHT + (poly->type == P_FLOOR)
		//	poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000
		};

		if (poly->type == P_FLOOR)
			fill_triangle(doom, 0xFF00FF, p, p2, p3);
		else
			fill_triangle(doom, 0xFFFFFF / (poly->vertices->vertices[0].x + poly->vertices->vertices[1].x + 1), p, p2, p3);
	/*	draw_line(&doom->screen, (t_pixel){p.x, p.y, 0x00FF00}, (t_pixel){p2.x, p2.y, 0xFF0000});
		draw_line(&doom->screen, (t_pixel){p2.x, p2.y,0x00FF00}, (t_pixel){p3.x, p3.y, 0xFF0000});
		draw_line(&doom->screen, (t_pixel){p3.x, p3.y, 0x00FF00}, (t_pixel){p.x, p.y, 0xFF0000});*/
		i += 3;
	}
}
