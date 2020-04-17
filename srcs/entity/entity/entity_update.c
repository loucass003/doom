/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 15:11:23 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/17 19:48:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "doom.h"

void		update_velocity(t_entity *entity)
{
	entity->velocity.x *= !entity->grounded && !entity->packet.found_colision
		&& !entity->jetpack ? 0.99 : 0.5;
	entity->velocity.z *= !entity->grounded && !entity->packet.found_colision
		&& !entity->jetpack ? 0.99 : 0.5;
	if (entity->velocity.x < 1 && entity->velocity.x > -1)
		entity->velocity.x = 0;
	if (entity->velocity.z < 1 && entity->velocity.z > -1)
		entity->velocity.z = 0;
}

void		set_entity_packet(t_doom *doom, t_entity *entity, double dt)
{
	entity->packet.r3_posision = entity->position;
	entity->packet.r3_velocity = entity->velocity;
	entity->packet.e_radius = entity->radius;
	entity->packet.grounded = FALSE;
	entity->packet.dt = dt;
	entity->packet.doom = doom;
}

void		update_ctx_editor(t_entity *entity, double dt)
{
	entity->grounded = FALSE;
	entity->position = ft_vec3_add(entity->position,
		ft_vec3_mul_s(entity->velocity, dt));
	entity->velocity = ft_vec3_mul_s(entity->velocity, 0.5);
}

t_bool		entity_update_ctx_normal(t_doom *doom, t_entity *entity, double dt)
{
	if (update_entity_type(doom, entity, dt) == FALSE)
		return (FALSE);
	set_entity_packet(doom, entity, dt);
	if (!collide_and_slide(entity))
		return (FALSE);
	if (entity->grounded)
		entity->jump = FALSE;
	entity->position = entity->packet.r3_posision;
	if (entity->type == ENTITY_GRENADA)
		entity->velocity = ft_vec3_mul_s(entity->velocity,
			!entity->packet.found_colision ? 0.999 : 0.8);
	else if (entity->type == ENTITY_ENEMY || entity->type == ENTITY_BOSS)
		entity->velocity = (t_vec3){ 0, 0, 0 };
	else if (entity->type == ENTITY_ROCKET)
		;
	else
		update_velocity(entity);
	entity->radius = entity->packet.e_radius;
	return (TRUE);
}

t_bool		entity_update(t_doom *doom, t_entity *entity, double dt)
{
	if (entity->position.y < -100)
		entity->life = 0;
	if (doom->main_context.type == CTX_NORMAL)
		if (entity_update_ctx_normal(doom, entity, dt) == FALSE)
			return (FALSE);
	if (doom->main_context.type == CTX_EDITOR)
		update_ctx_editor(entity, dt);
	return (TRUE);
}
