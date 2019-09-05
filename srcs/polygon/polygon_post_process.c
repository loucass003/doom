/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon_post_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:26:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/05 13:48:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "polygon.h"
#include "maths/triangle.h"
#include "doom.h"
#include "render.h"

static t_collidable	compute_face_collidable(t_renderable *r, t_face *face)
{
	t_collidable	col;
	t_collide_triangle	tri;

	col.type = COLLIDE_TRIANGLE;
	tri.parent_type = PARENT_COLLIDER_POLYGON;
	tri.polygon = r->of.data.polygon;
	tri.normal = face->face_normal;
	tri.points[0] = vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]);
	tri.points[1] = vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]);
	tri.points[2] = vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1]);
	tri.uv[0] = r->vertex->vertices[face->vertex_index[0] - 1];
	tri.uv[1] = r->vertex->vertices[face->vertex_index[1] - 1];
	tri.uv[2] = r->vertex->vertices[face->vertex_index[2] - 1];
	tri.v0v1 = ft_vec3_sub(tri.points[1], tri.points[0]);
	tri.v0v2 = ft_vec3_sub(tri.points[2], tri.points[0]);
	col.data.triangle = tri;
	return (col);
}

static t_bool	compute_normals(t_renderable *r)
{
	int				i;
	t_face			*face;

 	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		face->face_normal = get_triangle_normal(
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1])
		);
		r->pp_normals[face->vertices_index[0] - 1] = face->face_normal;
		r->pp_normals[face->vertices_index[1] - 1] = face->face_normal;
		r->pp_normals[face->vertices_index[2] - 1] = face->face_normal;
		face->collidable = compute_face_collidable(r, face);
	}
	return (TRUE);
}


t_bool			post_process_polygon(t_renderable *r)
{
	if (!r->pp_vertices && !(r->pp_vertices = malloc(sizeof(t_vec4) * r->vertices->len)))
		return (FALSE);
	if (!r->pp_normals && !(r->pp_normals = malloc(sizeof(t_vec3) * r->normals->len)))
		return (FALSE);
	if (!triangulate_polygon(r))
		return (FALSE);
	transform_renderable(r);
	if(!compute_normals(r))
		return (FALSE);
	return (TRUE);
}
