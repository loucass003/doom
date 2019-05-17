/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/17 02:38:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
//#define NEAR (1e-1)

/*int	max3(int a, int b, int c)
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
}*/

void	assemble_triangles(t_doom *doom, t_polygon *poly)
{
	const size_t	len = floorf(poly->indices->len / 3.);
	size_t			i;

	i = -1;
	while (++i < len)
	{
		if (ft_vec3_dot(poly->normals[i], ft_vec3_sub(doom->player.pos, poly->vertices->vertices[poly->indices->values[i * 3]])) <= 0)
			continue;
		process_triangle(doom, poly, (t_triangle3d){
			poly->pp_vertices[poly->indices->values[i * 3]],
			poly->pp_vertices[poly->indices->values[i * 3 + 1]],
			poly->pp_vertices[poly->indices->values[i * 3 + 2]]});
	}
}

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	int	i;

	i = -1;
	while (++i < poly->vertices->len)
		poly->pp_vertices[i] = ft_mat4_mulv(doom->player.matrix, poly->vertices->vertices[i]);
	assemble_triangles(doom, poly);
}