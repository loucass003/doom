/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 18:02:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/06 11:17:29 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static float		sign(float a)
{
	if (a > 0)
		return (1);
	else
		return (-1);
	return (0);
}


static t_vec4		compute_bounds(t_polygon *polygon)
{
	t_vec2	min;
	t_vec2	max;
	t_vec3	vert;
	t_vec2	dir;
	t_vec2	sub;
	int		i;


	max = (t_vec2){ INT_MIN, INT_MIN };
	min = (t_vec2){ INT_MAX, INT_MAX };
	sub = ft_vec2_sub(
		(t_vec2){polygon->vertices->vertices[1].x, polygon->vertices->vertices[1].y},
		(t_vec2){polygon->vertices->vertices[0].x, polygon->vertices->vertices[0].y}
	);
	dir = (t_vec2){sign(sub.x), sign(sub.y)};
	i = -1;
	while (++i < polygon->vertices->len)
	{
		vert = polygon->vertices->vertices[i];
		min.x = fmin(min.x, vert.x);
		min.y = fmin(min.y, vert.y);
		max.x = fmax(max.x, vert.x);
		max.y = fmax(max.y, vert.y);
	}
	return ((t_vec4){ .x = min.x, .y = min.y, .z = ft_absf(max.x - min.x), .w = ft_absf(max.y - min.y) });
}


void				uv_mapping(t_polygon *polygon)
{
	const t_vec4	bounds = compute_bounds(polygon);
	int				i;
	t_vec3			v;
	t_vec2			dir;
	t_vec2			sub;

	if (polygon->vertices->len < 2)
		return ; //TODO: Not supposed to happen
	sub = ft_vec2_sub(
		(t_vec2){polygon->vertices->vertices[1].x, polygon->vertices->vertices[1].y},
		(t_vec2){polygon->vertices->vertices[0].x, polygon->vertices->vertices[0].y}
	);
	dir = (t_vec2){sign(sub.x), sign(sub.y)};
	i = -1;
	printf("PLOP %f %f\n", dir.x, dir.y);
	while (++i < polygon->vertices->len)
	{
		v = polygon->vertices->vertices[i];
		printf("T %f %f\n", v.x, v.y);
		t_vec2 u = (t_vec2) {
			.y = (dir.y == 1)
				? 1 - ft_absf(v.x - bounds.x) / bounds.z
				: ft_absf(v.x - bounds.x) / bounds.z,
			.x = ft_absf(v.y - bounds.y) / bounds.w
		};
		printf("U %f %f\n", u.x, u.y);
		polygon->uvs[i] = u;
	}
	return ;
}