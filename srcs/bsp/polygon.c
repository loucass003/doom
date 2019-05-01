/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:30:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/01 16:45:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec2		get_polygon_normal(t_polygon *poly)
{
	t_vec3	*t1 = poly->vertices->values[2];
	t_vec3	*t2 = poly->vertices->values[1];
	t_vec3	a = ft_vec3_sub(*t1, *t2);

	t1 = poly->vertices->values[0];
	t2 = poly->vertices->values[1];
	t_vec3 b = ft_vec3_sub(*t1, *t2);
	t_vec3 c = ft_vec3_norm(ft_vec3_cross(a, b));
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
		t_vec3 *v1 = poly->vertices->values[i];
		int j = -1;
		while (++j < poly->vertices->len)
		{
			t_vec3 *v2 = poly->vertices->values[j];
			float d2 = distanceSq(v1->x, v1->z, v2->x, v2->z);
			if (d2 > d)
			{
				d = d2;
				line = (t_line){ .a = (t_vec2){v1->x, v1->z}, .b = (t_vec2){v2->x, v2->z}};
			}
		}
	}
	line.normal = get_polygon_normal(poly);
	return (line);
}

t_polygon	*clip_poly(t_polygon *poly, t_line partition, t_side side)
{
	t_arraylist	*vertices = create_arraylist(poly->vertices->len);
	int			i;

	i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		t_vec3 *v1 = (t_vec3 *)poly->vertices->values[i];
		t_vec3 *v2 = (t_vec3 *)poly->vertices->values[next];
		t_side side1 = get_side_thin(partition, (t_vec2){v1->x, v1->z});
		t_side side2 = get_side_thin(partition, (t_vec2){v2->x, v2->z});
		if (side1 != side)
			arraylist_appendm(&vertices, &(t_vec3){v1->x, v1->y, v1->z}, sizeof(t_vec3));
		if ((side1 == S_FRONT && side2 == S_BACK) 
			|| (side2 == S_FRONT && side1 == S_BACK))
		{
			if (v1->z > v2->z)
			{
				t_vec3 *tmp = v1;
				v1 = v2;
				v2 = tmp;
			}
			float f = get_intersection((t_line){ .a = {v1->x, v1->z}, .b = {v2->x, v2->z} }, partition);
			arraylist_appendm(&vertices, &(t_vec3){
				v1->x + f * (v2->x - v1->x),
				v1->y + f * (v2->y - v1->y),
				v1->z + f * (v2->z - v1->z)
			}, sizeof(t_vec3));
		}
	}
	i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		t_vec3 *v1 = (t_vec3 *)poly->vertices->values[i];
		t_vec3 *v2 = (t_vec3 *)poly->vertices->values[next];
		if (v1->x == v2->x && v1->y == v2->y && v1->z == v2->z)
			arraylist_splicef(vertices, i, 1, NULL);
	}
	if (vertices->len < 3)
	{
		arraylist_clear(&vertices, NULL);
		return (NULL);
	}
	return (create_polygon(vertices, poly->type));
}
