/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:30:01 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:30:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "item.h"

static t_bool		is_enemy(t_renderable *r)
{
	return (r->of.type == RENDERABLE_ENTITY
		&& (r->of.data.entity->type == ENTITY_ENEMY
			|| r->of.data.entity->type == ENTITY_BOSS));
}

void				on_use_weapon(t_doom *d, t_itemstack *is)
{
	t_weapon	*w;
	t_itemstack	*inv_is;
	t_collision	hit;
	int			slot;

	w = &is->of->data.weapon;
	if (w->fireing || (slot = get_slot_of(&d->player, ITEM_AMMO)) == -1)
		return ;
	inv_is = &d->player.item[slot];
	if (inv_is->amount > 0 && inv_is->amount--)
	{
		w->fireing = TRUE;
		player_sound(&d->audio, CHAR_SHOOTING, 1, 1.5);
		d->gameover.bullets += 1;
	}
	if (inv_is->amount <= 0)
		inv_is->of = NULL;
	hit = ray_hit_world(d, d->renderables,
		create_shoot_ray(d->player, (t_vec3){0, 0, 1}));
	if (hit.collide && is_enemy(hit.renderable))
	{
		give_damage(&d->player.entity, hit.renderable->of.data.entity,
			d, w->damage);
		hit.renderable->of.data.entity->of.enemy.focus = TRUE;
	}
}

void				on_use_axe(t_doom *doom, t_itemstack *is)
{
	t_collision	hit;
	t_weapon	*weapon;

	weapon = &is->of->data.weapon;
	if (weapon->fireing)
		return ;
	weapon->fireing = TRUE;
	hit = ray_hit_world(doom, doom->renderables, create_shoot_ray(doom->player,
		(t_vec3){0, 0, 1}));
	if (hit.collide && hit.dist < 2 && is_enemy(hit.renderable))
	{
		give_damage(&doom->player.entity, hit.renderable->of.data.entity, doom,
			weapon->damage);
		hit.renderable->of.data.entity->of.enemy.focus = TRUE;
	}
}
