/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:16:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/12 12:43:02 by lloncham         ###   ########.fr       */
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

int					get_slot_of(t_player *player, t_item_type type)
{
	int		i;

	i = -1;
	while (++i < PLAYER_INV_SIZE)
		if (player->item[i].of && player->item[i].of->type == type)
			return (i);
	return (-1);
}

int					get_slot(t_player *player, t_itemstack *is)
{
	return (get_slot_of(player, is->of->type));
}

int					get_empty_slot(t_player *player)
{
	int		i;

	i = -1;
	while (++i < PLAYER_INV_SIZE)
		if (!player->item[i].of)
			return (i);
	return (-1);
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
		entity->life = fmin(entity->max_life, entity->life + (0.5 * entity->packet.doom->level.coeff_regen));
		return (TRUE);
	}
	s = get_slot(&entity->packet.doom->player, is);
	if (s == -1 && (s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (TRUE);
	inv_is = &entity->packet.doom->player.item[s];
	if (inv_is->of && inv_is->of->type == ITEM_WEAPON &&  inv_is->of->data.weapon.type != WEAPON_GRENADA
		&& inv_is->of->data.weapon.type == is->of->data.weapon.type)
		return (TRUE);
	else if (inv_is->of && inv_is->of->type == ITEM_WEAPON &&  inv_is->of->data.weapon.type != WEAPON_GRENADA
		&& (s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (TRUE); 
	inv_is = &entity->packet.doom->player.item[s];
	inv_is->of = is->of;
	i = inv_is->of->max_stack_size - inv_is->amount;

	if (i > 0)
	{
		if (inv_is->of->type == ITEM_WEAPON)
			entity->packet.doom->gameover.weapon += ft_min(i, is->amount);
		inv_is->amount += ft_min(i, is->amount);
		is->amount -= ft_min(i, is->amount);
		player_sound(&entity->packet.doom->audio, ITEM_PICK, 3, 1);
		t_trigger t = (t_trigger) { .type = TRIG_PICK_ITEM };
		t.data.pick_item = (t_trigger_pick_item) {
			.item_type = is->of->type,
			.is_weapon_set = is->of->type == ITEM_WEAPON,
			.weapon_type = is->of->type == ITEM_WEAPON
				? is->of->data.weapon.type
				: WEAPON_NONE
		};
		trigger_event(entity->packet.doom, t);
	}
	return (TRUE);
}
