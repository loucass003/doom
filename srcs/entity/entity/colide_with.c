/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colide_with.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 21:14:21 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 19:48:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doom.h"
#include "octree.h"
#include <limits.h>

void			check_triangle_normal_type(t_entity_collision_check *data,
	t_face *f)
{
	t_vec3		points;

	(void)points;
	if (f->normal_type == 1 || f->normal_type == 2)
	{
		check_triangle(data->r, &data->entity->packet, (t_vec3[3]) {
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[
				f->vertices_index[0] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[
				f->vertices_index[1] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[
				f->vertices_index[2] - 1]), data->entity->packet.e_radius)
		});
	}
	if (f->normal_type == 0 || f->normal_type == 2)
	{
		check_triangle(data->r, &data->entity->packet, (t_vec3[3]) {
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[
				f->vertices_index[2] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[
				f->vertices_index[1] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[
				f->vertices_index[0] - 1]), data->entity->packet.e_radius)
		});
	}
}

void			collide_with_face(int face, void *p)
{
	t_entity_collision_check		*data;
	t_face							*f;

	data = p;
	f = &data->r->faces->values[face];
	if (!f->has_collision)
		return ;
	check_triangle_normal_type(data, f);
}

void			collide_with_octree(t_renderable *r, t_entity *entity,
	t_octree_node *octree, t_collide_aabb area)
{
	t_vec3 min;
	t_vec3 max;

	min = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	max = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
	min.x = fmin(min.x, area.min.x);
	min.y = fmin(min.y, area.min.y);
	min.z = fmin(min.z, area.min.z);
	min.x = fmin(min.x, area.max.x);
	min.y = fmin(min.y, area.max.y);
	min.z = fmin(min.z, area.max.z);
	max.x = fmax(max.x, area.min.x);
	max.y = fmax(max.y, area.min.y);
	max.z = fmax(max.z, area.min.z);
	max.x = fmax(max.x, area.max.x);
	max.y = fmax(max.y, area.max.y);
	max.z = fmax(max.z, area.max.z);
	area.min = min;
	area.max = max;
	aabb_intersect_octree(octree, &area, collide_with_face,
		&(t_entity_collision_check){ .entity = entity, .r = r });
}
