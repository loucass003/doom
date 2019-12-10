/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 18:02:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/10 14:36:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "render.h"

// static float		sign(float a)
// {
// 	if (a > 0)
// 		return (1);
// 	else
// 		return (-1);
// 	return (0);
// }

// static t_vec4		compute_bounds(t_4dvertices *vertices, int *filter, int filter_len)
// {
// 	t_vec2	min;
// 	t_vec2	max;
// 	t_vec4	vert;
// 	int		i;


// 	max = (t_vec2){ INT_MIN, INT_MIN };
// 	min = (t_vec2){ INT_MAX, INT_MAX };

// 	i = -1;
// 	while (++i < filter_len)
// 	{
// 		vert = vertices->vertices[filter[i]];
// 		min.x = fmin(min.x, vert.x);
// 		min.y = fmin(min.y, vert.y);
// 		max.x = fmax(max.x, vert.x);
// 		max.y = fmax(max.y, vert.y);
// 	}
// 	return ((t_vec4){ .x = min.x, .y = min.y, .z = ft_absf(max.x - min.x), .w = ft_absf(max.y - min.y) });
// }

void				uv_mapping(t_4dvertices *vertices, t_2dvertices *vertex, int *filter, int filter_len)
{
	// const t_vec4	bounds = compute_bounds(vertices, filter, filter_len);
	int				i;
	t_vec4			v;
	// t_vec2			dir;
	t_vec2			sub;

	if (filter_len < 3)
		return ;

	sub = ft_vec2_sub(
		(t_vec2){vertices->vertices[filter[1]].x, vertices->vertices[filter[1]].y},
		(t_vec2){vertices->vertices[filter[0]].x, vertices->vertices[filter[0]].y}
	);
	// dir = (t_vec2){sign(sub.x), sign(sub.y)};
	i = -1;
	while (++i < filter_len)
	{
		v = vertices->vertices[filter[i]];
		t_vec2 u = (t_vec2) {
			.y = v.y / sub.y,
			.x = v.x / sub.y
		};
	//	printf("U %f %f\n", u.x, u.y);
		vertex->vertices[filter[i]] = u;
	}
	return ;
}
