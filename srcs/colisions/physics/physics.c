/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 23:50:58 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/16 02:46:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths/triangle.h"
#include "collision.h"

typedef struct	s_plane 
{
	t_vec3	origin;
	t_vec3	normal;
	t_vec3	point;
	float	f;
}				t_plane;

t_plane	triangle_to_plane(t_vec3 a, t_vec3 b, t_vec3 c)
{
	const t_vec3	normal = get_triangle_normal(a, b, c);

	return ((t_plane) {
		.origin = a,
		.normal = normal,
		.point = normal,
		.f = -ft_vec3_dot(normal, a)
	});
}

t_plane	plane_new(t_vec3 origin, t_vec3 normal)
{
	return ((t_plane){
		.origin = origin,
		.normal = normal,
		.point = normal,
		.f = -ft_vec3_dot(normal, origin)
	});
}

float	distance_to_plane(t_vec3 point, t_plane plane)//TODO: probably need fix 
{
	return ft_vec3_dot(point, plane.normal) + plane.f;
}

t_bool	is_front_facing(t_plane plane, t_vec3 direction)//TODO: probably need fix 
{
	const float f = ft_vec3_dot(plane.normal, direction);
	if (f <= 0) //TODO: probably inversed
		return (TRUE);
	return (FALSE);
}

t_bool	point_in_triangle(t_vec3 point, t_vec3 p1, t_vec3 p2, t_vec3 p3)
{
	const t_vec3	u = ft_vec3_sub(p2, p1);//TODO: probably inversed too
	const t_vec3	v = ft_vec3_sub(p3, p1);
	const t_vec3	w = ft_vec3_sub(point, p1);
	const t_vec3	vw = ft_vec3_cross(v, w);
	const t_vec3	vu = ft_vec3_cross(v, u);

	if (ft_vec3_dot(vw, vu) < 0)
		return (FALSE);
	
	t_vec3 uw = ft_vec3_cross(u, w);
	t_vec3 uv = ft_vec3_cross(u, v);
	if (ft_vec3_dot(uw, uv) < 0)
		return (FALSE);
	
	float d = 1.0 / ft_vec3_len(uv);
	float r = ft_vec3_len(vw) * d;
	float t = ft_vec3_len(uw) * d;

	return ((r + t) <= 1);
}

t_bool	lowest_root(t_vec3 v, float max, float *root)
{
	const float det = v.y * v.y - 4.0 * v.x * v.z;

	if (det < 0)
		return (FALSE);
	float sqrt_d = sqrtf(det);
	float r1 = (-v.y - sqrt_d) / (2.0 * v.x);
	float r2 = (-v.y + sqrt_d) / (2.0 * v.x);
	if (r1 > r2) 
	{
		float tmp = r2;
		r2 = r1;
		r1 = tmp;
	}
	if (r1 > 0 && r1 < max)
	{
		*root = r1;
		return (TRUE);
	}
	if (r2 > 0 && r2 < max)
	{
		*root = r2;
		return (TRUE);
	}
	return (FALSE);
}

