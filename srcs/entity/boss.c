/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boss.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/28 14:15:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/19 17:28:35 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <math.h>
#include <stdlib.h>

t_bool	create_boss_renderable(t_doom *doom, t_renderable *r)
{
	t_sprite	*sprite;
	t_entity	*boss;

	if (!(sprite = create_sprite((t_vec2){ 7, 4 },
		doom->res_manager.ressources->values[6])))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	set_current_cell(r, 0, 0);
	r->of.type = RENDERABLE_ENTITY;
	if (!(boss = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	boss->type = ENTITY_BOSS;
	boss->packet.doom = doom;
	boss->scale = (t_vec3){ 5, 5, 5 };
	boss->radius = (t_vec3){ 2.5, 2.5, 2.5 };
	boss->life = 8;
	boss->killable = TRUE;
	boss->max_life = 8;
	boss->sources = boss->of.boss.sources;
	alGenSources(4, boss->sources);
	r->of.data.entity = boss;
	r->scale = boss->scale;
	compute_entity_hitbox(r);
	return (TRUE);
}

void	boss_update_sound(t_entity *entity)
{
	ALint status;

	if ((entity->velocity.x || entity->velocity.z) && entity->grounded)
	{
		alGetSourcei(entity->sources[2], AL_SOURCE_STATE, &status);
		if (status != AL_PLAYING)
			entity_sound(entity, 4, 2, 1);
	}
}

void	entity_update_boss(t_doom *doom, t_entity *entity, double dt)
{
	t_entity_boss	*boss;

	boss = &entity->of.boss;
	if (entity->dead)
		return ;
	if (boss->phase == 0 && entity->life < entity->max_life / 2)
	{
		boss->animation_step = 0;
		boss->phase = 3;
	}
	if (boss->phase == 1 && entity->life <= 0)
	{
		boss->animation_step = 0;
		boss->phase = 2;
		entity_sound(entity, 6, 0, 1);
	}
	boss_update_step(doom, entity, boss, dt);
	boss_update_sound(entity);
}
