/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_post_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:26:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/04 13:49:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collision.h"
#include "maths/triangle.h"
#include "obj.h"

t_collidable	compute_face_collidable(t_obj *obj, t_face *face)
{
	t_collidable	col;
	t_collide_triangle	tri;

	col.type = COLLIDE_TRIANGLE;
	tri.parent_type = PARENT_COLLIDER_OBJ;
	tri.obj = obj;
	tri.normal = face->face_normal;
	tri.points[0] = vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1]);
	tri.points[1] = vec4_to_3(obj->pp_vertices[face->vertices_index[1] - 1]);
	tri.points[2] = vec4_to_3(obj->pp_vertices[face->vertices_index[2] - 1]);
	tri.uv[0] = obj->vertex->vertices[face->vertex_index[0] - 1];
	tri.uv[1] = obj->vertex->vertices[face->vertex_index[1] - 1];
	tri.uv[2] = obj->vertex->vertices[face->vertex_index[2] - 1];
	col.data.triangle = tri;
	return (col);
}

void		transfom_obj(t_obj *obj)
{
	int		i;

	const	t_mat4 rot = ft_mat4_rotation(obj->rotation);
	const	t_mat4 m = ft_mat4_mul(
		ft_mat4_mul(
			ft_mat4_translation(obj->position),
			rot
		),
		ft_mat4_scale(obj->scale)
	); 

	i = -1;
	while (++i < obj->vertices->len)
		obj->pp_vertices[i] = mat4_mulv4(m, obj->vertices->vertices[i]);
	i = -1;
	while (++i < obj->normals->len)
		obj->pp_normals[i] = ft_mat4_mulv(rot, obj->normals->vertices[i]);
	i = -1;
	while (++i < obj->faces->len)
	{
		t_face *face = &obj->faces->values[i];
		face->face_normal = get_triangle_normal(
			vec4_to_3(obj->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(obj->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(obj->pp_vertices[face->vertices_index[2] - 1]));
	}
}

t_bool			post_process_obj(t_obj *obj)
{
	int			i;
	t_face		*face;

	transfom_obj(obj);
	i = -1;
	while (++i < obj->faces->len)
	{
		face = &obj->faces->values[i];
		face->collidable = compute_face_collidable(obj, face);
	}
	return (TRUE);
}
