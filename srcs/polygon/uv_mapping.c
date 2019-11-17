/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 18:02:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/16 21:28:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "render.h"

static float		sign(float a)
{
	if (a > 0)
		return (1);
	else
		return (-1);
	return (0);
}

static t_vec4		compute_bounds(t_4dvertices *vertices)
{
	t_vec2	min;
	t_vec2	max;
	t_vec4	vert;
	int		i;


	max = (t_vec2){ INT_MIN, INT_MIN };
	min = (t_vec2){ INT_MAX, INT_MAX };

	i = -1;
	while (++i < vertices->len)
	{
		vert = vertices->vertices[i];
		min.x = fmin(min.x, vert.x);
		min.y = fmin(min.y, vert.y);
		max.x = fmax(max.x, vert.x);
		max.y = fmax(max.y, vert.y);
	}
	return ((t_vec4){ .x = min.x, .y = min.y, .z = ft_absf(max.x - min.x), .w = ft_absf(max.y - min.y) });
}

void				uv_mapping(t_4dvertices *vertices, t_2dvertices *vertex)
{
	const t_vec4	bounds = compute_bounds(vertices);
	int				i;
	t_vec4			v;
	t_vec2			dir;
	t_vec2			sub;

	if (vertices->len < 2)
		return ; //TODO: Not supposed to happen
	sub = ft_vec2_sub(
		(t_vec2){vertices->vertices[1].x, vertices->vertices[1].y},
		(t_vec2){vertices->vertices[0].x, vertices->vertices[0].y}
	);
	dir = (t_vec2){sign(sub.x), sign(sub.y)};
	i = -1;
//	printf("PLOP %f %f\n", dir.x, dir.y);
	while (++i < vertices->len)
	{
		v = vertices->vertices[i];
		//printf("T %f %f\n", v.x, v.y);
		t_vec2 u = (t_vec2) {
			.y = (dir.y == 1)
				? ft_absf(v.x - bounds.x) / bounds.z
				: 1 - ft_absf(v.x - bounds.x) / bounds.z,
			.x = (dir.y == 1)
				? ft_absf(v.y - bounds.y) / bounds.w
				: 1 - ft_absf(v.y - bounds.y) / bounds.w
		};
	//	printf("U %f %f\n", u.x, u.y);
		vertex->vertices[i] = u;
	}
	return ;
}
