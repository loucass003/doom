/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/17 02:43:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_vec3	transform(t_vec3 v)
{
	const float	z_inv = -1. / v.z;

	v.x = (v.x * z_inv + 1.) * S_WIDTH_2;
	v.y = (v.y * z_inv + 1.) * S_HEIGHT_2;
	return v;
}

void	process_triangle(t_doom *doom, t_polygon *poly, t_triangle3d triangle)
{
	triangle.a = transform(triangle.a);
	triangle.b = transform(triangle.b);
	triangle.c = transform(triangle.c);

	draw_triangle(doom, triangle);
}