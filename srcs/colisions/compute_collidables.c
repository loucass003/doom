/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_collidables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:00:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/08 00:26:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "collision.h"
#include "render.h"

t_collidable	face_collidable(t_renderable *r, int face_index, t_vec4 *vertices)
{
	t_collidable	col;
	t_collide_triangle	tri;
	t_face				*face;

	face = &r->faces->values[face_index];
	col.type = COLLIDE_TRIANGLE;
	tri.face = face_index;
	tri.points[0] = vec4_to_3(vertices[face->vertices_index[0] - 1]);
	tri.points[1] = vec4_to_3(vertices[face->vertices_index[1] - 1]);
	tri.points[2] = vec4_to_3(vertices[face->vertices_index[2] - 1]);

	t_vec3 n = get_triangle_normal(tri.points[0], tri.points[1], tri.points[2]);
	tri.double_sided = face->double_sided || r->double_faced;
	if (face->normal_type == 1)
		tri.normal = ft_vec3_inv(n);
	else
		tri.normal = n;
	if (r->vertex)
	{
		tri.uv[0] = r->vertex->vertices[face->vertex_index[0] - 1];
		tri.uv[1] = r->vertex->vertices[face->vertex_index[1] - 1];
		tri.uv[2] = r->vertex->vertices[face->vertex_index[2] - 1];
	}
	tri.v0v1 = ft_vec3_sub(tri.points[1], tri.points[0]);
	tri.v0v2 = ft_vec3_sub(tri.points[2], tri.points[0]);
	col.data.triangle = tri;
	col.of = r->of;
	return (col);
}

void			compute_collidables(t_renderable *r)
{
	t_face		*face;
	int			i;

	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		face->collidable = face_collidable(r, i, r->vertices->vertices);
	}
}