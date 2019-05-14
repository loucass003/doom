/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:30:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/14 16:14:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec2		get_polygon_normal(t_polygon *poly)
{
	const t_vec3 p0 = poly->vertices->vertices[0];
	const t_vec3 p1 = poly->vertices->vertices[1];
	const t_vec3 p2 = poly->vertices->vertices[2];
	const t_vec3 c = get_plane_normal(p0, p1, p2);
	
	return ((t_vec2){ c.x, c.z });
}

double distanceSq(double var0, double var2, double var4, double var6) {
	var0 -= var4;
	var2 -= var6;
	return var0 * var0 + var2 * var2;
}


t_line		get_poly_line(t_polygon *poly)
{
	if (poly->type != P_WALL)
		exit(-1);
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
	line.normal = get_polygon_normal(poly);
	return (line);
}

t_bool	clip_poly(t_polygon *out, t_polygon *poly, t_line partition, t_side side)
{
	t_3dvertices	*vertices = create_3dvertices_array(poly->vertices->len);
	int			i;

	i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		t_vec3 v1 = poly->vertices->vertices[i];
		t_vec3 v2 = poly->vertices->vertices[next];
		t_side side1 = get_side_thin(partition, (t_vec2){v1.x, v1.z});
		t_side side2 = get_side_thin(partition, (t_vec2){v2.x, v2.z});
		if (side1 != side)
			append_3dvertices_array(&vertices, v1);
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
			append_3dvertices_array(&vertices, (t_vec3){
				v1.x + f * (v2.x - v1.x),
				v1.y + f * (v2.y - v1.y),
				v1.z + f * (v2.z - v1.z)
			});
		}
	}
	i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		t_vec3 v1 = poly->vertices->vertices[i];
		t_vec3 v2 = poly->vertices->vertices[next];
		if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
			splice_3dvertices_array(vertices, i, 1);
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
