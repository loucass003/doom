/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 16:36:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/19 15:28:31 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <stdlib.h>

t_bool		create_enemy_renderable(t_doom *doom, t_renderable *r)
{
	t_sprite	*sprite;
	t_entity	*enemy;

	if (!(sprite = create_sprite((t_vec2){ 8, 7 },
		doom->res_manager.ressources->values[5])))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	set_current_cell(r, 0, 0);
	r->of.type = RENDERABLE_ENTITY;
	if (!(enemy = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	enemy->type = ENTITY_ENEMY;
	enemy->packet.doom = doom;
	enemy->scale = (t_vec3){ 5, 5, 5 };
	enemy->radius = (t_vec3){ 1, 2.5, 1 };
	enemy->life = 1;
	enemy->killable = TRUE;
	enemy->sources = enemy->of.enemy.sources;
	alGenSources(3, enemy->sources);
	r->of.data.entity = enemy;
	r->scale = enemy->scale;
	compute_entity_hitbox(r);
	return (TRUE);
}

void		entity_update_enemy(t_doom *doom, t_entity *entity, double dt)
{
	t_entity_enemy	*enemy;

	enemy = &entity->of.enemy;
	if (entity->dead)
		return ;
	if (entity->life <= 0 && !entity->diying)
	{
		entity->diying = TRUE;
		enemy->animation_step = 5;
		entity_sound(entity, 0, 0, 2);
	}
	enemy->t0 += 5 * dt;
	entity_anim_step(doom, entity, enemy);
	entity_update_sound(entity);
}
