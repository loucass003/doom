/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 01:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/22 23:08:18 by llelievr         ###   ########.fr       */
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

static void 	clip_2(t_doom *doom, t_polygon *poly, t_triangle4d t)
{
	const float	alpha_a = (-t.a.z) / (t.c.z - t.a.z);
	const float	alpha_b = (-t.b.z) / (t.c.z - t.b.z);
	const t_vec4 v_a = ft_vec4_interpolate(t.a, t.c, alpha_a);
	const t_vec4 v_b = ft_vec4_interpolate(t.b, t.c, alpha_b);

	post_process_triangle(doom, poly, (t_triangle4d){ v_a, v_b, t.c });
}

static void 	clip_1(t_doom *doom, t_polygon *poly, t_triangle4d t)
{
	const float	alpha_a = (-t.a.z) / (t.b.z - t.a.z);
	const float	alpha_b = (-t.a.z) / (t.c.z - t.a.z);
	const t_vec4 v_a = ft_vec4_interpolate(t.a, t.b, alpha_a);
	const t_vec4 v_b = ft_vec4_interpolate(t.a, t.c, alpha_b);

	post_process_triangle(doom, poly, (t_triangle4d){ v_a, t.b, t.c });
	post_process_triangle(doom, poly, (t_triangle4d){ v_b, v_a, t.c });
}

void	clip_triangle(t_doom *doom, t_polygon *poly, t_triangle4d t)
{
 	if (t.a.x < t.a.w && t.b.x < t.b.w && t.c.x < t.c.w)
		return;
	if (t.a.x > -t.a.w && t.b.x > -t.b.w && t.c.x > -t.c.w)
		return;
	 if (t.a.y < t.a.w && t.b.y < t.b.w && t.c.y < t.c.w)
		return;
	if (t.a.y > -t.a.w && t.b.y < -t.b.w && t.c.y > -t.c.w)
		return;
	if (t.a.z < t.a.w && t.b.z < t.b.w && t.c.z < t.c.w)
		return;
	if (t.a.z < 0 && t.b.z < 0 && t.c.z < 0)
		return;
/*	if (t.a.z < 0)
	{
		if (t.b.z < 0)
			clip_2(doom, poly, t);
		else if (t.c.z < 0)
			clip_2(doom, poly, (t_triangle4d){t.a, t.c, t.b});
		else
			clip_1(doom, poly, t);
	}
	else if (t.b.z < 0)
	{
		if (t.c.z < 0)
			clip_2(doom, poly, (t_triangle4d){t.b, t.c, t.a});
		else
			clip_1(doom, poly, (t_triangle4d){t.b, t.a, t.c});
	}
	else if (t.c.z < 0)
		clip_1(doom, poly, (t_triangle4d){t.c, t.a, t.b});
	else*/
		post_process_triangle(doom, poly, t);
}

void	process_triangle(t_doom *doom, t_polygon *poly, t_triangle4d triangle)
{
	clip_triangle(doom, poly, triangle);
}

void	post_process_triangle(t_doom *doom, t_polygon *poly, t_triangle4d triangle)
{
	triangle.a = transform(triangle.a);
	triangle.b = transform(triangle.b);
	triangle.c = transform(triangle.c);

	draw_triangle(doom, (t_triangle3d){
		{ .pos = vec4_to_3(triangle.a), .tex = {0, 0} },
		{ .pos = vec4_to_3(triangle.b), .tex = {0, 1} },
		{ .pos = vec4_to_3(triangle.c), .tex = {1, 1} }
	});
}