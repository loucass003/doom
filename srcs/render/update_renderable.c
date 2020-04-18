/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_renderable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/17 23:18:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 12:47:29 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "entity.h"
#include "sprite.h"

void	update_renderable_entity_enemy(t_render_context *ctx, t_renderable *r,
	t_entity *entity)
{
	t_sprite		*sprite;
	t_entity_enemy	*enemy;
	t_vec3			view;
	float			a;
	int				texture;

	enemy = &entity->of.enemy;
	sprite = r->sprite;
	view = ft_vec3_sub(ctx->camera->pos, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	a = atan2(view.z, view.x);
	texture = round(a * (4 / M_PI));
	if (enemy->animation_step == 6)
		texture = enemy->shooting ? 2 : 1;
	if (entity->diying)
		texture = enemy->diying_step;
	set_current_cell(r, texture, enemy->animation_step);
}

void	update_renderable_entity_boss(t_renderable *r, t_entity *entity)
{
	t_entity_boss	*boss;

	boss = &entity->of.boss;
	set_current_cell(r, boss->animation_step, boss->phase);
}

t_bool	update_renderable_entity(t_render_context *ctx, t_renderable *r)
{
	t_entity	*entity;

	entity = r->of.data.entity;
	r->position = entity->position;
	r->rotation = entity->rotation;
	r->dirty = TRUE;
	if (entity->type == ENTITY_ENEMY)
		update_renderable_entity_enemy(ctx, r, entity);
	else if (entity->type == ENTITY_BOSS)
		update_renderable_entity_boss(r, entity);
	return (TRUE);
}
