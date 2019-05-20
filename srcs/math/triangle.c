/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/20 15:18:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_vec4	transform(t_vec4 v)
{
	const float	z_inv = 1. / v.z;

	v.x = (v.x * z_inv + 1.) * S_WIDTH_2;
	v.y = (v.y * z_inv + 1.) * S_HEIGHT_2;
	return v;
}

void	process_triangle(t_doom *doom, t_polygon *poly, t_triangle4d triangle)
{
	triangle.a.pos = transform(triangle.a.pos);
	triangle.b.pos = transform(triangle.b.pos);
	triangle.c.pos = transform(triangle.c.pos);

	draw_triangle(doom, triangle);
}
