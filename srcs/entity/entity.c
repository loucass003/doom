/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:00:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 17:37:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "doom.h"
#include "maths/plane.h"
#include "entity.h"
#include "render.h"
#include "sprite.h"
#include <SDL.h>

void		teleport(t_entity *entity, t_vec3 pos, t_vec3 rot)
{
	entity->position = pos;
	entity->rotation = rot;
	entity->ontp = FALSE;
	player_sound(&entity->packet.doom->audio, CHAR_TELEPORT, 10, 1);
}

t_bool		renderable_transpo(t_entity *entity)
{
	t_transpo	*transpo;
	t_vec3		pos;

	transpo = entity->packet.r->of.data.transpo;
	if (entity->ontp == FALSE)
	{
		entity->ontp = TRUE;
		transpo->cooldown = SDL_GetTicks();
	}
	if (SDL_GetTicks() - transpo->cooldown >= 5000)
	{
		pos = editor_to_world(entity->packet.doom->editor.objects
			->values[transpo->connected].pos);
		pos.y += 1.5 + entity->radius.y;
		teleport(entity, pos, entity->rotation);
		return (FALSE);
	}
	return (TRUE);
}

void		compute_entity_hitbox(t_renderable *r)
{
	const t_entity	*entity = r->of.data.entity;

	compute_ellipsoid_hitbox(r, entity->position, entity->radius);
}
