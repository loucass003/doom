/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/02 18:16:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_vec4	transform(t_vec4 v)
{
	const float	z_inv = -1. / v.z;
	v.x = (v.x * z_inv + 1.) * S_WIDTH_2;
	v.y = (v.y * z_inv + 1.) * S_HEIGHT_2;
	v.z = -z_inv;
	return v;
}

static void 	clip_2(t_doom *doom, t_polygon *poly, t_triangle t)
{
	const float	alpha_a = (NEAR - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR - t.b.pos.z) / (t.c.pos.z - t.b.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.c, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.b, t.c, alpha_b);
	
	post_process_triangle(doom, poly, (t_triangle){ v_a, v_b, t.c });
}

static void 	clip_1(t_doom *doom, t_polygon *poly, t_triangle t)
{
	const float	alpha_a = (NEAR - t.a.pos.z) / (t.b.pos.z - t.a.pos.z);
	const float	alpha_b = (NEAR - t.a.pos.z) / (t.c.pos.z - t.a.pos.z);
	const t_vertex v_a = vertex_interpolate(t.a, t.b, alpha_a);
	const t_vertex v_b = vertex_interpolate(t.a, t.c, alpha_b);

	post_process_triangle(doom, poly, (t_triangle){ v_a, t.b, t.c });
	post_process_triangle(doom, poly, (t_triangle){ v_b, v_a, t.c });
}

void	clip_triangle(t_doom *doom, t_polygon *poly, t_triangle t)
{
 /*	if (t.a.pos.x < t.a.pos.w && t.b.pos.x < t.b.pos.w && t.c.pos.x < t.c.pos.w)
		return;
	if (t.a.pos.x > -t.a.pos.w && t.b.pos.x > -t.b.pos.w && t.c.pos.x > -t.c.pos.w)
		return;
	 if (t.a.pos.y < t.a.pos.w && t.b.pos.y < t.b.pos.w && t.c.pos.y < t.c.pos.w)
		return;
	if (t.a.pos.y > -t.a.pos.w && t.b.pos.y < -t.b.pos.w && t.c.pos.y > -t.c.pos.w)
		return;
	if (t.a.pos.z < t.a.pos.w && t.b.pos.z < t.b.pos.w && t.c.pos.z < t.c.pos.w)
		return;*/
	if (t.a.pos.z < NEAR && t.b.pos.z < NEAR && t.c.pos.z < NEAR)
		return;
	if (t.a.pos.z < NEAR)
	{
		if (t.b.pos.z < NEAR)
			clip_2(doom, poly, t);
		else if (t.c.pos.z < NEAR)
			clip_2(doom, poly, (t_triangle){t.a, t.c, t.b});
		else
			clip_1(doom, poly, t);
	}
	else if (t.b.pos.z < NEAR)
	{
		if (t.c.pos.z < NEAR)
			clip_2(doom, poly, (t_triangle){t.b, t.c, t.a});
		else
			clip_1(doom, poly, (t_triangle){t.b, t.a, t.c});
	}
	else if (t.c.pos.z < NEAR)
		clip_1(doom, poly, (t_triangle){t.c, t.a, t.b});
	else
		post_process_triangle(doom, poly, t);
}

void	process_triangle(t_doom *doom, t_polygon *poly, t_triangle triangle)
{
	clip_triangle(doom, poly, triangle);
}

void	post_process_triangle(t_doom *doom, t_polygon *poly, t_triangle triangle)
{
	triangle.a.pos = transform(ft_vec4_div_s(triangle.a.pos, triangle.a.pos.w));
	triangle.b.pos = transform(ft_vec4_div_s(triangle.b.pos, triangle.b.pos.w));
	triangle.c.pos = transform(ft_vec4_div_s(triangle.c.pos, triangle.c.pos.w));

	draw_triangle(doom, triangle);
}
