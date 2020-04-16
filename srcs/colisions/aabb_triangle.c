/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 04:34:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 17:27:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "collision.h"
#include <math.h>

t_bool		plane_box_overlap(t_vec3 normal, t_vec3 vert, t_vec3 maxbox)
{
	t_vec3_u	v_min;
	t_vec3_u	v_max;
	int			q;
	float		v;

	q = -1;
	while (++q < 3)
	{
		v = ((t_vec3_u)vert).a[q];
		if (((t_vec3_u)normal).a[q] > 0.0f)
		{
			v_min.a[q] = -((t_vec3_u)maxbox).a[q] - v;
			v_max.a[q] = ((t_vec3_u)maxbox).a[q] - v;
		}
		else
		{
			v_min.a[q] = ((t_vec3_u)maxbox).a[q] - v;
			v_max.a[q] = -((t_vec3_u)maxbox).a[q] - v;
		}
	}
	if (ft_vec3_dot(normal, v_min.v) > 0.0f)
		return (TRUE);
	if (ft_vec3_dot(normal, v_max.v) >= 0.0f)
		return (TRUE);
	return (FALSE);
}

t_bool		axis_tests(t_vec3 e[3], t_vec2 bh[3], t_vec3 v[3], t_vec2 *range)
{
	t_vec3 fe;

	fe = (t_vec3){ fabsf(e[0].x), fabsf(e[0].y), fabsf(e[0].z) };
	if (!axis_test((t_vec4){ e[0].z, e[0].y, fe.z, fe.y },
		(t_vec4){ v[0].y, v[0].z, v[2].y, v[2].z }, bh[0], range)
		|| !axis_test((t_vec4){ e[0].z, e[0].x, fe.z, fe.x },
			(t_vec4){ v[0].x, v[0].z, v[2].x, v[2].z }, bh[1], range)
		|| !axis_test((t_vec4){ e[0].y, e[0].x, fe.y, fe.x },
			(t_vec4){ v[1].x, v[1].y, v[2].x, v[2].y }, bh[2], range))
		return (FALSE);
	fe = (t_vec3){ fabsf(e[1].x), fabsf(e[1].y), fabsf(e[1].z) };
	if (!axis_test((t_vec4){ e[1].z, e[1].y, fe.z, fe.y }, (t_vec4){ v[0].y,
		v[0].z, v[2].y, v[2].z }, bh[0], range) || !axis_test((t_vec4){ e[1].z,
		e[1].x, fe.z, fe.x }, (t_vec4){ v[0].x, v[0].z, v[2].x, v[2].z },
		bh[1], range) || !axis_test((t_vec4){ e[1].y, e[1].x, fe.y, fe.x },
		(t_vec4){ v[0].x, v[0].y, v[1].x, v[1].y }, bh[2], range))
		return (FALSE);
	fe = (t_vec3){ fabsf(e[2].x), fabsf(e[2].y), fabsf(e[2].z) };
	if (!axis_test((t_vec4){ e[2].z, e[2].y, fe.z, fe.y }, (t_vec4){ v[0].y,
		v[0].z, v[1].y, v[1].z }, bh[0], range) || !axis_test((t_vec4){ e[2].z,
		e[2].x, fe.z, fe.x }, (t_vec4){ v[0].x, v[0].z, v[1].x, v[1].z },
		bh[1], range) || !axis_test((t_vec4){ e[2].y, e[2].x, fe.y, fe.x },
		(t_vec4){ v[1].x, v[1].y, v[2].x, v[2].y }, bh[2], range))
		return (FALSE);
	return (TRUE);
}

t_bool		findmaxes(t_vec3 v[3], t_vec3 boxhalfsize, t_vec2 *range)
{
	findmax((t_vec3){v[0].x, v[1].x, v[2].x}, range);
	if (range->x > boxhalfsize.x || range->y < -boxhalfsize.x)
		return (FALSE);
	findmax((t_vec3){v[0].y, v[1].y, v[2].y}, range);
	if (range->x > boxhalfsize.y || range->y < -boxhalfsize.y)
		return (FALSE);
	findmax((t_vec3){v[0].z, v[1].z, v[2].z}, range);
	if (range->x > boxhalfsize.z || range->y < -boxhalfsize.z)
		return (FALSE);
	return (TRUE);
}

t_bool		triangle_overlap_box(t_vec3 boxcenter, t_vec3 boxhalfsize,
	t_vec3 triverts[3])
{
	t_vec3 v[3];
	t_vec3 e[3];
	t_vec2 range;
	t_vec3 normal;
	t_vec2 bh[3];

	bh[0] = (t_vec2){ boxhalfsize.y, boxhalfsize.z };
	bh[1] = (t_vec2){ boxhalfsize.x, boxhalfsize.z };
	bh[2] = (t_vec2){ boxhalfsize.x, boxhalfsize.y };
	v[0] = ft_vec3_sub(triverts[0], boxcenter);
	v[1] = ft_vec3_sub(triverts[1], boxcenter);
	v[2] = ft_vec3_sub(triverts[2], boxcenter);
	e[0] = ft_vec3_sub(v[1], v[0]);
	e[1] = ft_vec3_sub(v[2], v[1]);
	e[2] = ft_vec3_sub(v[0], v[2]);
	if (!axis_tests(e, bh, v, &range) || !findmaxes(v, boxhalfsize, &range))
		return (FALSE);
	normal = ft_vec3_cross(e[0], e[1]);
	if (!plane_box_overlap(normal, v[0], boxhalfsize))
		return (FALSE);
	return (TRUE);
}

t_collision	triangle_hit_aabb(t_collide_triangle *triangle,
	t_collide_aabb *aabb)
{
	t_vec3 half;

	half = ft_vec3_mul_s(ft_vec3_sub(aabb->max, aabb->min), 0.5);
	half.x = fabs(half.x);
	half.y = fabs(half.y);
	half.z = fabs(half.z);
	if (triangle_overlap_box(ft_vec3_add(aabb->min, half),
		ft_vec3_mul_s(half, 1.01), triangle->points))
		return ((t_collision){ .collide = TRUE, .dist = 1 });
	else
		return (t_collision){ .collide = FALSE, .dist = 1 };
}
