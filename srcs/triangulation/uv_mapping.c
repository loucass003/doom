/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 18:02:59 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/25 15:34:43 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "render.h"

void				uv_mapping(t_4dvertices *vertices, t_2dvertices *vertex,
	int *filter, int filter_len, t_vec2 offset, t_vec2 repeat)
{
	int				i;
	t_vec4			v;
	t_vec2			sub;

	if (filter_len < 3)
		return ;
	sub = ft_vec2_norm(ft_vec2_sub(
	(t_vec2){vertices->vertices[filter[1]].x, vertices->vertices[filter[1]].y},
	(t_vec2){vertices->vertices[filter[0]].x, vertices->vertices[filter[0]].y}
	));
	i = -1;
	while (++i < filter_len)
	{
		v = vertices->vertices[filter[i]];
		t_vec2 u = (t_vec2) {
			.y = (v.y * 0.5 * repeat.y) + offset.y,
			.x = (v.x * 0.5 * repeat.x) + offset.x
		};
		vertex->vertices[filter[i]] = u;
	}
	return ;
}
