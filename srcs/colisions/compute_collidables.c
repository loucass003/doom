/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_collidables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:00:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/28 17:03:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "collision.h"
#include "render.h"

static t_collide_aabb	compute_face_box(t_renderable *r, int face_index, t_vec4 *vertices)
{
	int				i;
	t_collide_aabb	box;
	t_vec4			vert;

	box.max = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
	box.min = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	i = -1;
	while (++i < 3)
	{
		vert = vertices[r->faces->values[face_index].vertices_index[i] - 1];
		box.min.x = fmin(box.min.x, vert.x);
		box.min.y = fmin(box.min.y, vert.y);
		box.min.z = fmin(box.min.z, vert.z);
		box.max.x = fmax(box.max.x, vert.x);
		box.max.y = fmax(box.max.y, vert.y);
		box.max.z = fmax(box.max.z, vert.z);
	}
	return (box);
}

t_collidable	compute_obj_face_collidable(t_renderable *r, int face_index, t_vec4 *vertices)
{
	t_collidable	col;
	t_collide_triangle	tri;
	t_face				*face;

	face = &r->faces->values[face_index];
	col.type = COLLIDE_TRIANGLE;
	tri.parent_type = PARENT_COLLIDER_OBJ;
	tri.obj = r->of.data.obj;
	tri.face = face_index;
	tri.points[0] = vec4_to_3(vertices[face->vertices_index[0] - 1]);
	tri.points[1] = vec4_to_3(vertices[face->vertices_index[1] - 1]);
	tri.points[2] = vec4_to_3(vertices[face->vertices_index[2] - 1]);
	tri.normal = get_triangle_normal(tri.points[0], tri.points[1], tri.points[2]);
	tri.uv[0] = r->vertex->vertices[face->vertex_index[0] - 1];
	tri.uv[1] = r->vertex->vertices[face->vertex_index[1] - 1];
	tri.uv[2] = r->vertex->vertices[face->vertex_index[2] - 1];
	tri.v0v1 = ft_vec3_sub(tri.points[1], tri.points[0]);
	tri.v0v2 = ft_vec3_sub(tri.points[2], tri.points[0]);
	tri.use_box = TRUE;
	tri.box = compute_face_box(r, face_index, vertices);
	col.data.triangle = tri;
	return (col);
}

t_collidable	compute_polygon_face_collidable(t_renderable *r, int face_index, t_vec4 *vertices)
{
	t_collidable	col;
	t_collide_triangle	tri;
	t_face				*face;

	face = &r->faces->values[face_index];
	col.type = COLLIDE_TRIANGLE;
	tri.parent_type = PARENT_COLLIDER_POLYGON;
	tri.face = face_index;
	tri.polygon = r->of.data.polygon;
	tri.points[0] = vec4_to_3(vertices[face->vertices_index[0] - 1]);
	tri.points[1] = vec4_to_3(vertices[face->vertices_index[1] - 1]);
	tri.points[2] = vec4_to_3(vertices[face->vertices_index[2] - 1]);
	tri.normal = get_triangle_normal(tri.points[0], tri.points[1], tri.points[2]);
	tri.uv[0] = r->vertex->vertices[face->vertex_index[0] - 1];
	tri.uv[1] = r->vertex->vertices[face->vertex_index[1] - 1];
	tri.uv[2] = r->vertex->vertices[face->vertex_index[2] - 1];
	tri.v0v1 = ft_vec3_sub(tri.points[1], tri.points[0]);
	tri.v0v2 = ft_vec3_sub(tri.points[2], tri.points[0]);
	tri.use_box = TRUE;
	tri.box = compute_face_box(r, face_index, vertices);
	col.data.triangle = tri;
	return (col);
}

t_collidable	compute_collidable(t_renderable *r, int face_index, t_vec4 *vertices)
{
	if (r->of.type == RENDERABLE_OBJ)
		return (compute_obj_face_collidable(r, face_index, vertices));
	else if (r->of.type == RENDERABLE_POLYGON)
		return (compute_polygon_face_collidable(r, face_index, vertices));
	else
	{
		// printf("HUM ? NOT GOOD\n");
		return ((t_collidable){});
	}
}
