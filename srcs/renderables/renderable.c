/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 00:02:57 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/02 15:40:02 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "render.h"
#include "entity.h"

void		transform_renderable_all(t_renderable *r)
{
	int				i;
	const t_mat4	tmp = ft_mat4_rotation(r->rotation);
	const t_mat4	rot = r->rot_matrix_set ? r->rot_matrix : tmp;

	r->matrix = ft_mat4_mul(
		ft_mat4_mul(ft_mat4_translation(r->position), rot),
		ft_mat4_scale(r->scale));
	i = -1;
	while (++i < r->vertices->len)
		r->pp_vertices[i] = mat4_mulv4(r->matrix, r->vertices->vertices[i]);
	i = -1;
	while (++i < r->normals->len)
		r->pp_normals[i] = ft_mat4_mulv(rot, r->normals->vertices[i]);
	i = -1;
	while (++i < r->faces->len)
		face_and_triangle(r, i);
}

void		transform_renderable(t_renderable *r)
{
	if (r->of.type == RENDERABLE_DOOR)
	{
		transform_renderable_door(r);
		return ;
	}
	transform_renderable_all(r);
}

t_bool		create_renderable(t_renderable *r, t_renderable_type type)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = type;
	r->object_index = -1;
	r->visible = TRUE;
	if (!(r->vertices = create_4dvertices_array(4)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->vertex = create_2dvertices_array(4)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->normals = create_3dvertices_array(4)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->faces = create_faces_array(2)))
		return (free_renderable(r, FALSE, FALSE));
	if (!(r->materials = create_mtllist(4)))
		return (free_renderable(r, FALSE, FALSE));
	return (TRUE);
}

t_bool		copy_renderable_malloc(t_renderable *src, t_renderable *dest)
{
	if (!(dest->pp_vertices = malloc(sizeof(t_vec4) * src->vertices->len)))
		return (FALSE);
	if (!(dest->pp_normals = malloc(sizeof(t_vec3) * src->normals->len)))
		return (FALSE);
	return (TRUE);
}

t_bool		copy_renderable(t_renderable *src, t_renderable *dest)
{
	ft_memcpy(dest, src, sizeof(t_renderable));
	if (!(dest->vertices = create_4dvertices_array(src->vertices->capacity)))
		return (FALSE);
	ft_memcpy(dest->vertices, src->vertices, sizeof(t_4dvertices)
		+ sizeof(t_vec4) * src->vertices->capacity);
	if (src->vertex)
	{
		if (!(dest->vertex = create_2dvertices_array(src->vertex->capacity)))
			return (FALSE);
		ft_memcpy(dest->vertex, src->vertex, sizeof(t_2dvertices)
			+ sizeof(t_vec2) * src->vertex->capacity);
	}
	if (!(dest->normals = create_3dvertices_array(src->normals->capacity)))
		return (FALSE);
	ft_memcpy(dest->normals, src->normals, sizeof(t_3dvertices)
		+ sizeof(t_vec3) * src->normals->capacity);
	if (!(dest->faces = create_faces_array(src->faces->capacity)))
		return (FALSE);
	ft_memcpy(dest->faces, src->faces, sizeof(t_faces)
		+ sizeof(t_face) * src->faces->capacity);
	if (copy_renderable_malloc(src, dest) == FALSE)
		return (FALSE);
	transform_renderable(src);
	return (TRUE);
}
