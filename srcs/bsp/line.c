/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:29:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/14 23:08:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool		get_partition(t_polygons *polygons, t_line *partition)
{
	int			i;
	t_polygon	*polygon;

	i = -1;
	while (++i < polygons->len)
	{
		polygon = &polygons->polygons[i];
		if (polygon->type == P_WALL)
		{
			t_line line = get_poly_line(polygon);
			partition->a = line.a;
			partition->b = line.b;
			partition->normal = line.normal;
			return (TRUE);
		}
	}
	return (FALSE);
}

t_side		get_side_thin(t_line partition, t_vec2 v)
{
	const t_vec2	n = partition.normal;
	const float		side = (v.x - partition.a.x) * n.x
		+ (v.y - partition.a.y) * n.y;
	if (side < 0)
		return (S_BACK);
	else if (side > 0)
		return (S_FRONT);
	else 
		return (S_COLINEAR);
}

t_side		get_side_thick(t_line partition, t_vec2 v)
{
	const t_vec2	n = partition.normal;
	const t_side	front = get_side_thin(partition, (t_vec2){ v.x - n.x / 2.,
		v.y - n.y / 2.});
	
	if (front == S_FRONT)
		return (S_FRONT);
	else if (front == S_BACK && get_side_thin(partition, (t_vec2){ v.x + n.x / 2., v.y + n.y / 2.}) == S_BACK)
		return (S_BACK);
	return (S_COLINEAR);
}

t_side		get_poly_side(t_line partition, t_polygon *poly)
{
	t_bool	front = FALSE;
	t_bool	back = FALSE;
	t_vec3	vec;
	t_side	side;
	int		i;

	i = -1;
	while (++i < poly->vertices->len)
	{
		vec = poly->vertices->vertices[i];
		side = get_side_thick(partition, (t_vec2){vec.x, vec.z});
		if (side == S_FRONT)
			front = TRUE;
		else if (side == S_BACK)
			back = TRUE;
	}
	if (front && back)
		return S_SPANNING;
	else if (front)
		return (S_FRONT);
	else if (back)
		return (S_BACK);
	else
		return (S_COLINEAR);
}

float		get_intersection(t_line a, t_line b)
{
	const float	d = (b.b.y - b.a.y) * (a.b.x - a.a.x)
		- (b.b.x - b.a.x) * (a.b.y - a.a.y);
	float		n;

	if (d == 0)
		return (-1);
	n = (b.b.x - b.a.x) * (a.a.y - b.a.y)
		- (b.b.y - b.a.y) * (a.a.x - b.a.x);
	return (n / d);
}
