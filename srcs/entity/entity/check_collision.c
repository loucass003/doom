/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_collision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 21:13:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 21:31:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doom.h"

t_collide_aabb	check_col_octree(t_collide_aabb new_area, t_renderable r,
	t_entity *entity, int i)
{
	t_physics_data	data;

	data = entity->packet;
	new_area.min = point_to_local(new_area.min, r.position, r.rotation,
		r.scale);
	new_area.max = point_to_local(new_area.max, r.position, r.rotation,
		r.scale);
	collide_with_octree(&r, entity, r.octree, new_area);
	if (data.distance > entity->packet.distance)
		entity->packet.r = &entity->packet.doom->renderables->values[i];
	return (new_area);
}

void			check_col_else(t_renderable r, t_entity *entity)
{
	int	j;

	j = -1;
	while (++j < r.faces->len)
		collide_with_face(j, &(t_entity_collision_check){
			.entity = entity, .r = &r });
}

void			check_col(t_renderable r, t_entity *entity,
	t_collide_aabb new_area, int i)
{
	if (r.has_hitbox && r.hitbox.type == COLLIDE_ELLIPSOID)
		check_col_collide_ellipsoid(r, entity, new_area, i);
	else if (r.octree && r.faces->len > 100)
		new_area = check_col_octree(new_area, r, entity, i);
	else
		check_col_else(r, entity);
}

t_bool				check_collision(t_entity *entity, t_collide_aabb area)
{
	int				i;
	t_renderable	r;
	t_collide_aabb	new_area;

	i = -1;
	while (++i < entity->packet.doom->renderables->len)
	{
		r = entity->packet.doom->renderables->values[i];
		entity->packet.r = NULL;
		if (r.of.data.entity == entity || (r.of.type == RENDERABLE_ENTITY
			&& (r.of.data.entity->type == ENTITY_GRENADA
				|| (r.of.data.entity->dead))))
			continue;
		if (entity->type == ENTITY_ROCKET && r.of.type == RENDERABLE_ENTITY
			&& r.of.data.entity->type == ENTITY_BOSS)
			continue;
		if (entity->type == ENTITY_GRENADA && r.of.type == RENDERABLE_ENTITY
			&& r.of.data.entity->type == ENTITY_PLAYER)
			continue;
		new_area = area;
		check_col(r, entity, new_area, i);
		entity_hit_something(entity, &i);
	}
	return (TRUE);
}
