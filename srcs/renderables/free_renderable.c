/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_renderable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 15:39:35 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/15 03:29:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "render.h"
#include "entity.h"

void		free_materials(t_mtllist **m_addr, t_bool free_ressources)
{
	int		i;
	t_mtl	*mtl;

	if (!(*m_addr))
		return ;
	i = -1;
	while (++i < (*m_addr)->len)
	{
		mtl = &(*m_addr)->values[i];
		if (mtl->texture_map_set && mtl->texture_map && free_ressources)
		{
			destroy_image(mtl->texture_map);
			ft_memdel((void **)&mtl->texture_map);
		}
	}
	ft_memdel((void **)m_addr);
}

void		free_renderable_of(t_renderable *r)
{
	if (r->of.type == RENDERABLE_ELLIPSOID)
		ft_memdel((void **)&r->of.data.ellipsoid);
	else if (r->of.type == RENDERABLE_OBJ)
		free_obj((void **)&r->of.data.obj, FALSE);
	else if (r->of.type == RENDERABLE_ENTITY
		&& r->of.data.entity->type != ENTITY_PLAYER)
		ft_memdel((void **)&r->of.data.entity);
	else if (r->of.type == RENDERABLE_ITEMSTACK)
		ft_memdel(&r->of.data.itemstack);
	else if (r->of.type == RENDERABLE_TRANSPO)
		ft_memdel((void **)&r->of.data.transpo);
	else if (r->of.type == RENDERABLE_EXPLOSION)
		ft_memdel((void **)&r->of.data.explosion);
	else if (r->of.type == RENDERABLE_DOOR)
		free_door(&r->of.data.door);
}

t_bool		free_renderable(t_renderable *r, t_bool free_resources,
	t_bool free_of, t_bool res)
{
	ft_memdel((void **)&r->vertices);
	ft_memdel((void **)&r->pp_vertices);
	ft_memdel((void **)&r->pp_normals);
	ft_memdel((void **)&r->faces);
	ft_memdel((void **)&r->vertex);
	ft_memdel((void **)&r->normals);
	if (r->object_index == -1
		|| (r->object_index != -1
			&& (r->of.type == RENDERABLE_ITEMSTACK
				|| r->of.type == RENDERABLE_ENTITY)))
		ft_memdel((void **)&r->sprite);
	if (r->materials && (free_resources || !r->model))
		free_materials(&r->materials, free_resources);
	if (r->octree && (free_resources || !r->model))
		free_octree(&r->octree);
	if (r->object_index == -1
		|| (r->object_index != -1
			&& (r->of.type == RENDERABLE_ITEMSTACK
				|| r->of.type == RENDERABLE_ELLIPSOID
				|| r->of.type == RENDERABLE_ENTITY)))
		free_renderable_of(r);
	return (res);
}

void		face_and_triangle(t_renderable *r, int i)
{
	t_face	*face;
	t_vec3	n;

	face = &r->faces->values[i];
	n = get_triangle_normal(
		vec4_to_3(r->pp_vertices[face->vertices_index[0] - 1]),
		vec4_to_3(r->pp_vertices[face->vertices_index[1] - 1]),
		vec4_to_3(r->pp_vertices[face->vertices_index[2] - 1]));
	if (face->normal_type == 0)
		face->face_normal = ft_vec3_inv(n);
	else
		face->face_normal = n;
}

void		free_renderables(t_renderables **renderables, t_bool destroy_array)
{
	int				i;
	t_renderable	*r;

	if (!(*renderables))
		return ;
	i = -1;
	while (++i < (*renderables)->len)
	{
		r = &(*renderables)->values[i];
		free_renderable(r, FALSE, FALSE, TRUE);
	}
	if (destroy_array)
		ft_memdel((void **)renderables);
}
