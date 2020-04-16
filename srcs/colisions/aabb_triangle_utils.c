/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_triangle_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:25:24 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 17:25:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "collision.h"
#include <math.h>

t_bool	axis_test(t_vec4 params, t_vec4 v, t_vec2 v3, t_vec2 *range)
{
	float	p0;
	float	p1;
	float	rad;

	p0 = params.x * v.x - params.y * v.y;
	p1 = params.x * v.z - params.y * v.z;
	if (p0 < p1)
	{
		range->x = p0;
		range->y = p1;
	}
	else
	{
		range->x = p1;
		range->y = p0;
	}
	rad = params.z * v3.x + params.w * v3.y;
	if (range->x > rad || range->y < -rad)
		return (FALSE);
	return (TRUE);
}

void	findmax(t_vec3 v, t_vec2 *range)
{
	range->x = v.x;
	range->y = v.x;

	if (v.y < range->x)
		range->x = v.y;
	if (v.y > range->y)
		range->y = v.y;
	if (v.z < range->x)
		range->x = v.z;
	if (v.z > range->y)
		range->y = v.z;
}
