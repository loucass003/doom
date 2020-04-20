/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 18:02:59 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/20 23:56:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "render.h"

void				uv_mapping(t_triangulate *t, t_2dvertices *vertex)
{
	int				i;
	t_vec4			v;

	if (t->filter_len < 3)
		return ;
	i = -1;
	while (++i < t->filter_len)
	{
		v = t->vertices->vertices[t->filter[i]];
		vertex->vertices[t->filter[i]] = (t_vec2) {
			.y = (v.y * 0.5 * t->repeat.y) + t->offset.y,
			.x = (v.x * 0.5 * t->repeat.x) + t->offset.x
		};
	}
	return ;
}
