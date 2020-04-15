/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 21:46:59 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 21:51:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collision.h"

t_bool		point_in_triangle_cross(const t_vec3 u, const t_vec3 v,
	const t_vec3 w, const t_vec3 vw)
{
	t_vec3	uw;
	t_vec3	uv;
	float	d;
	float	r;
	float	t;

	uw = ft_vec3_cross(u, w);
	uv = ft_vec3_cross(u, v);
	if (ft_vec3_dot(uw, uv) < 0)
		return (FALSE);
	d = ft_vec3_len(uv);
	r = ft_vec3_len(vw) / d;
	t = ft_vec3_len(uw) / d;
	return ((r + t) <= 1);
}

t_bool		point_in_triangle(t_vec3 point, t_vec3 p1, t_vec3 p2, t_vec3 p3)
{
	const t_vec3	u = ft_vec3_sub(p2, p1);
	const t_vec3	v = ft_vec3_sub(p3, p1);
	const t_vec3	w = ft_vec3_sub(point, p1);
	const t_vec3	vw = ft_vec3_cross(v, w);
	const t_vec3	vu = ft_vec3_cross(v, u);

	if (ft_vec3_dot(vw, vu) < 0)
		return (FALSE);
	return (point_in_triangle_cross(u, v, w, vw));
}

void		swapf(float *a, float *b)
{
	float	t;

	t = *a;
	*a = *b;
	*b = t;
}

t_bool		lowest_root(t_vec3 v, float max, float *root)
{
	const float	det = v.y * v.y - 4.0 * v.x * v.z;
	float		sqrt_d;
	float		r1;
	float		r2;

	if (det < 0)
		return (FALSE);
	sqrt_d = sqrtf(det);
	r1 = (-v.y - sqrt_d) / (2.0 * v.x);
	r2 = (-v.y + sqrt_d) / (2.0 * v.x);
	if (r1 > r2)
		swapf(&r1, &r2);
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

float		clamp(float min, float max, float v)
{
	return (fmin(max, fmax(min, v)));
}
