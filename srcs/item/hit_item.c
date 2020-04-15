/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:16:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:59:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "render.h"
#include "renderable_of.h"
#include "sprite.h"
#include "entity.h"
#include "doom.h"
#include "item.h"
#include "script.h"

t_bool				is_weapon_not_grenada(t_itemstack *it)
{
	return (it && it->of && it->of->type == ITEM_WEAPON
		&& it->of->data.weapon.type != WEAPON_GRENADA);
}

int					get_item_slot(t_entity *entity, t_itemstack *is)
{
	int				s;
	t_itemstack		*inv_is;

	s = get_slot(&entity->packet.doom->player, is);
	if (s == -1 && (s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (-1);
	inv_is = &entity->packet.doom->player.item[s];
	if (is_weapon_not_grenada(inv_is)
		&& inv_is->of->data.weapon.type == is->of->data.weapon.type)
		return (-1);
	else if (is_weapon_not_grenada(inv_is)
		&& (s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (-1);
	return (s);
}

void				consume_item(t_itemstack *inv_is, t_itemstack *is,
	t_entity *entity, int i)
{
	t_trigger t;

	if (inv_is->of->type == ITEM_WEAPON)
		entity->packet.doom->gameover.weapon += ft_min(i, is->amount);
	inv_is->amount += ft_min(i, is->amount);
	is->amount -= ft_min(i, is->amount);
	player_sound(&entity->packet.doom->audio, ITEM_PICK, 3, 0.6);
	t = (t_trigger) { .type = TRIG_PICK_ITEM };
	t.data.pick_item = (t_trigger_pick_item) {
		.item_type = is->of->type,
		.is_weapon_set = is->of->type == ITEM_WEAPON,
		.weapon_type = is->of->type == ITEM_WEAPON
			? is->of->data.weapon.type
			: WEAPON_NONE
	};
	trigger_event(entity->packet.doom, t);
}

t_bool				entity_hit_itemstack(t_entity *entity, t_itemstack *is)
{
	int				s;
	int				i;
	t_itemstack		*inv_is;

	if (&entity->packet.doom->player.entity != entity)
		return (TRUE);
	if (is->of && is->of->type == ITEM_HEAL)
	{
		is->amount--;
		entity->life = fmin(entity->max_life, entity->life + (0.5
			* entity->packet.doom->level.coeff_regen));
		return (TRUE);
	}
	if ((s = get_item_slot(entity, is)) == -1)
		return (TRUE);
	inv_is = &entity->packet.doom->player.item[s];
	inv_is->of = is->of;
	i = inv_is->of->max_stack_size - inv_is->amount;
	if (i > 0)
		consume_item(inv_is, is, entity, i);
	return (TRUE);
}
