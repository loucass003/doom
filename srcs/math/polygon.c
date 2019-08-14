/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:30:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/14 13:51:15 by llelievr         ###   ########.fr       */
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
