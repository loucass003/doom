/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 00:02:57 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/07 16:06:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "render.h"
#include "entity.h"

t_bool		free_renderable(t_renderable **r, t_bool res)
{
	*r = NULL;
	return (res);
}

void		transform_renderable(t_renderable *r)
{
	int		i;

	if (r->of.type == RENDERABLE_DOOR)
	{
		transform_renderable_door(r);
		return ;
	}
	
	
	const t_mat4 rot = r->rot_matrix_set ? r->rot_matrix : ft_mat4_rotation(r->rotation);
	r->matrix = ft_mat4_mul(
		ft_mat4_mul(
			ft_mat4_translation(r->position),
			rot
		),
		ft_mat4_scale(r->scale)
	);


	i = -1;
	while (++i < r->vertices->len)
		r->pp_vertices[i] = mat4_mulv4(r->matrix, r->vertices->vertices[i]);
	i = -1;
	while (++i < r->normals->len)
		r->pp_normals[i] = ft_mat4_mulv(rot, r->normals->vertices[i]);
	i = -1;
	while (++i < r->faces->len)
	{
		t_face *face = &r->faces->values[i];
		t_vec3	n = get_triangle_normal(
			vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]),
			vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1]));
	
		if (face->normal_type == 0)
			face->face_normal = ft_vec3_inv(n);
		else
			face->face_normal = n;
	}
}


t_bool		create_renderable(t_renderable	*r, t_renderable_type type)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = type;
	r->object_index = -1;
	r->visible = TRUE;
	if(!(r->vertices = create_4dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->vertex = create_2dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->normals = create_3dvertices_array(4)))
		return (free_renderable(&r, FALSE));
	if(!(r->faces = create_faces_array(2)))
		return (free_renderable(&r, FALSE));
	if(!(r->materials = create_mtllist(4)))
		return (free_renderable(&r, FALSE));
	return (TRUE);
}


t_bool		copy_renderable(t_renderable *src, t_renderable *dest)
{
	ft_memcpy(dest, src, sizeof(t_renderable));
	if (!(dest->vertices = create_4dvertices_array(src->vertices->capacity)))
		return (FALSE);
	ft_memcpy(dest->vertices, src->vertices, sizeof(t_4dvertices) + sizeof(t_vec4) * src->vertices->capacity);
	if (src->vertex)
	{
		if (!(dest->vertex = create_2dvertices_array(src->vertex->capacity)))
			return (FALSE);
		ft_memcpy(dest->vertex, src->vertex, sizeof(t_2dvertices) + sizeof(t_vec2) * src->vertex->capacity);
	}
	if (!(dest->normals = create_3dvertices_array(src->normals->capacity)))
		return (FALSE);
	ft_memcpy(dest->normals, src->normals, sizeof(t_3dvertices) + sizeof(t_vec3) * src->normals->capacity);
	if (!(dest->faces = create_faces_array(src->faces->capacity)))
		return (FALSE);
	ft_memcpy(dest->faces, src->faces, sizeof(t_faces) + sizeof(t_face) * src->faces->capacity);
	if (!(dest->pp_vertices = malloc(sizeof(t_vec4) * src->vertices->len)))
		return (FALSE);
	if (!(dest->pp_normals = malloc(sizeof(t_vec3) * src->normals->len)))
		return (FALSE);
	transform_renderable(src);
	return (TRUE);
}
