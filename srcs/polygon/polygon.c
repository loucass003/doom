/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:30:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 02:49:33 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "polygon.h"
#include "maths/triangle.h"

t_vec3		get_polygon_normal(t_polygon *poly)
{
	const t_vec3 p0 = poly->vertices->vertices[0];
	const t_vec3 p1 = poly->vertices->vertices[1];
	const t_vec3 p2 = poly->vertices->vertices[2];
	const t_vec3 c = get_triangle_normal(p0, p1, p2);
	
	return ((t_vec3){ c.x, c.y, c.z });
}

static double distanceSq(double x0, double y0, double x1, double y1) {
	x0 -= x1;
	y0 -= y1;
	return x0 * x0 + y0 * y0;
}

