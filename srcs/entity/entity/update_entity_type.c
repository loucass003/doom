/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_entity_type.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 15:12:59 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/15 17:03:23 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "doom.h"
#include "maths/plane.h"
#include "entity.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"
#include <SDL.h>

void		update_jetpack(t_doom *doom, t_entity *entity)
{
	int			slot;
	t_itemstack	*is;

	entity->jetpack = FALSE;
	slot = get_slot_of(&doom->player, ITEM_JETPACK);
	if (slot != -1)
	{
		is = &doom->player.item[slot];
		if (entity->type == ENTITY_PLAYER && is->of
			&& is->of->type == ITEM_JETPACK && !entity->grounded)
		{
			is->amount--;
			if (is->amount <= 0)
				is->of = NULL;
			entity->jetpack = TRUE;
		}
	}
}

void		update_velocity_jump(t_entity *entity)
{
	if (entity->jetpack)
	{
		if (entity->jump)
			entity->velocity.y += 1.5;
		entity->jump = FALSE;
		entity->velocity.y *= 0.9;
	}
	else
	{
		if (entity->grounded && entity->jump && entity->velocity.y < 0)
			entity->velocity.y = fmax(0, entity->velocity.y);
		if (entity->jump && entity->velocity.y <= 12
			&& !entity->packet.found_colision)
			entity->velocity.y += 1.5;
		if (entity->jump && entity->velocity.y >= 12)
			entity->jump = FALSE;
		if (!entity->jump || entity->run)
		{
			entity->velocity.y -= !entity->grounded ? 8 : 40;
			entity->velocity.y = fmax(-40, entity->velocity.y);
		}
	}
}

t_bool		update_entity_type(t_doom *doom, t_entity *entity, double dt)
{
	if (entity->type == ENTITY_BOSS)
		entity_update_boss(doom, entity, dt);
	if (entity->type == ENTITY_ENEMY)
		entity_update_enemy(doom, entity, dt);
	if (entity->type == ENTITY_GRENADA)
		if (!entity_update_grenada(doom, entity))
			return (FALSE);
	update_jetpack(doom, entity);
	if (entity->type == ENTITY_PLAYER && (entity->velocity.x
		|| entity->velocity.z) && entity->grounded
		&& doom->audio.source_status[CHAR_FOOTSTEP] == 0)
		player_sound(&doom->audio, CHAR_FOOTSTEP, 2, 0.25);
	if (entity->type == ENTITY_GRENADA)
	{
		entity->velocity.y -= 0.9;
		entity->velocity.y = fmax(-4, entity->velocity.y);
	}
	else if (entity->type == ENTITY_ROCKET)
		;
	else
		update_velocity_jump(entity);
	return (TRUE);
}
