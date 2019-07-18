/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:30:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/16 17:41:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec3		get_polygon_normal(t_polygon *poly)
{
	const t_vec3 p0 = poly->vertices->vertices[0];
	const t_vec3 p1 = poly->vertices->vertices[1];
	const t_vec3 p2 = poly->vertices->vertices[2];
	const t_vec3 c = get_plane_normal(p0, p1, p2);
	
	return ((t_vec3){ c.x, c.y, c.z });
}

static double distanceSq(double x0, double y0, double x1, double y1) {
	x0 -= x1;
	y0 -= y1;
	return x0 * x0 + y0 * y0;
}

t_line		get_poly_line(t_polygon *poly)
{
	float	d = -1;
	int		i = -1;
	t_line	line = (t_line){(t_vec2){-1, -1}, (t_vec2){-1, -1}};
	while (++i < poly->vertices->len)
	{
		t_vec3 v1 = poly->vertices->vertices[i];
		int j = -1;
		while (++j < poly->vertices->len)
		{
			t_vec3 v2 = poly->vertices->vertices[j];
			float d2 = distanceSq(v1.x, v1.z, v2.x, v2.z);
			if (d2 > d)
			{
				d = d2;
				line = (t_line){ .a = (t_vec2){v1.x, v1.z}, .b = (t_vec2){v2.x, v2.z}};
			}
		}
	}
	t_vec3 n = get_polygon_normal(poly);
	line.normal = (t_vec2){n.x, n.z};
	return (line);
}

t_bool	clip_poly(t_polygon *out, t_polygon *poly, t_line partition, t_side side)
{
	t_3dvertices	*vertices = create_3dvertices_array(poly->vertices->len);
	int				i;

	i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		t_vec3 v1 = poly->vertices->vertices[i];
		t_vec3 v2 = poly->vertices->vertices[next];
		t_side side1 = get_side_thin(partition, (t_vec2){v1.x, v1.z});
		t_side side2 = get_side_thin(partition, (t_vec2){v2.x, v2.z});
		if (side1 != side)
		{
			//printf("V1 %f %f %f\n", v1.x, v1.y, v1.z);
			append_3dvertices_array(&vertices, v1);
		}
		if ((side1 == S_FRONT && side2 == S_BACK) 
			|| (side2 == S_FRONT && side1 == S_BACK))
		{
			if (v1.z > v2.z)
			{
				t_vec3 tmp = v1;
				v1 = v2;
				v2 = tmp;
			}
			float f = get_intersection((t_line){ .a = {v1.x, v1.z}, .b = {v2.x, v2.z} }, partition);
			t_vec3 t = (t_vec3){
				v1.x + f * (v2.x - v1.x),
				v1.y + f * (v2.y - v1.y),
				v1.z + f * (v2.z - v1.z)
			};
		//	printf ("T %f %f %f\n", t.x, t.y, t.z);
			append_3dvertices_array(&vertices, t);
		}
	}
	i = -1;
	while (++i < vertices->len)
	{
		int next = (i + 1) % vertices->len;
		t_vec3 v1 = vertices->vertices[i];
		t_vec3 v2 = vertices->vertices[next];
		if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
		{
		//	printf("rv %f %f %f\n", v1.x, v1.y, v1.z);
			splice_3dvertices_array(vertices, i, 1);
		/*	if (i < vertices->len && i > 0)
				printf("rve %f %f %f\n", vertices->vertices[i].x, vertices->vertices[i].y, vertices->vertices[i].z);*/
		
		}
	}
	if (vertices->len < 3)
	{
		free(vertices);
		vertices = NULL;
		return (FALSE);
	}
	*out = create_polygon(vertices, poly->type);
	return (TRUE);
}

t_bounds3	get_polygon_bounds(t_polygon *polygon)
{
	t_vec3	min;
	t_vec3	max;
	t_vec3	vert;
	int		i;

	max = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
	min = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	i = -1;
	while (++i < polygon->vertices->len)
	{
		vert = polygon->vertices->vertices[i];
		min.x = fmin(min.x, vert.x);
		min.y = fmin(min.y, vert.y);
		min.z = fmin(min.z, vert.z);
		max.x = fmax(max.x, vert.x);
		max.y = fmax(max.y, vert.y);
		max.z = fmax(max.z, vert.z);
	}
	return ((t_bounds3){ .pos = min, .size = ft_vec3_sub(max, min) });
}