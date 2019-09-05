/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 17:31:15 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/05 21:14:37 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "octree.h"
#include "collision.h"
#include "render.h"

t_collidable	get_maximum_box(t_renderable *r)
{
	t_collidable	col;
	t_collide_aabb	box;
	t_vec4			vert;
	int				i;

	col.type = COLLIDE_AABB;
	box.min = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
	box.max = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	i = -1;
	while (++i < r->vertices->len)
	{
		vert = r->vertices->vertices[i];
		box.min.x = fmin(box.min.x, vert.x);
		box.min.y = fmin(box.min.y, vert.y);
		box.min.z = fmin(box.min.z, vert.z);
		box.max.x = fmax(box.max.x, vert.x);
		box.max.y = fmax(box.max.y, vert.y);
		box.max.z = fmax(box.max.z, vert.z);
	}
	col.data.aabb = box;
	return (col);
}