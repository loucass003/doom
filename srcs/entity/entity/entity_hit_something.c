/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_hit_something.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 21:27:37 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 21:28:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doom.h"

void			check_col_collide_ellipsoid(t_renderable r, t_entity *entity,
	t_collide_aabb new_area, int i)
{
	t_collide_ellipsoid	ellipsoid;
	t_renderable		*sphere;
	t_physics_data		data;

	data = entity->packet;
	ellipsoid = r.hitbox.data.ellipsoid;
	sphere = &entity->packet.doom->sphere_primitive;
	sphere->position = ellipsoid.origin;
	sphere->scale = ellipsoid.radius;
	sphere->dirty = TRUE;
	transform_renderable(sphere);
	new_area.min = point_to_local(new_area.min, ellipsoid.origin,
		(t_vec3){0, 0, 0}, ellipsoid.radius);
	new_area.max = point_to_local(new_area.max, ellipsoid.origin,
		(t_vec3){0, 0, 0}, ellipsoid.radius);
	collide_with_octree(sphere, entity, sphere->octree, new_area);
	if (data.distance > entity->packet.distance)
		entity->packet.r = &entity->packet.doom->renderables->values[i];
}

t_bool			check_collision_rocket(t_entity *entity, t_physics_data data)
{
	entity_sound(entity, 8, 0, 1);
	damage_explo(entity, entity->packet.doom, entity->of.rocket.damage);
	splice_renderables_array(entity->packet.doom->renderables,
		renderables_indexof(entity->packet.doom->renderables,
			entity->r), 1);
	entity->packet = data;
	return (FALSE);
}

void				check_col_itemstack(t_entity *entity, int *i,
	t_physics_data data)
{
	if (entity_hit_itemstack(entity,
		entity->packet.r->of.data.itemstack))
	{
		if (entity->packet.r->of.data.itemstack->amount <= 0)
		{
			splice_renderables_array(entity->packet.doom->renderables, *i, 1);
			(*i)--;
		}
		entity->packet = data;
	}
}

t_bool				entity_hit_something(t_entity *entity, int *i)
{
	if (entity->packet.r
		&& entity->packet.r->of.type == RENDERABLE_ITEMSTACK)
		check_col_itemstack(entity, i, entity->packet);
	if (entity->type == ENTITY_ROCKET && entity->packet.found_colision)
		return (check_collision_rocket(entity, entity->packet));
	if (entity->packet.r && entity->packet.r->of.type == RENDERABLE_TRANSPO
		&& check_collision_transpo(entity) == FALSE)
		return (FALSE);
	return (TRUE);
}
