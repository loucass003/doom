/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:37:22 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:04:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "item.h"
#include "player.h"
#include "doom.h"

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

t_item				*create_item_from_type(t_doom *doom, t_item_type type,
	t_weapon_type weapon_type)
{
	if (type == ITEM_AMMO)
		return (create_item_ammo(doom->res_manager.ressources->values[2]));
	else if (type == ITEM_HEAL)
		return (create_item_heal(doom->res_manager.ressources->values[2]));
	else if (type == ITEM_JETPACK)
		return (create_item_jetpack(doom->res_manager.ressources->values[2]));
	else if (type == ITEM_WEAPON)
		return (create_item_weapon_from_type(doom, weapon_type));
	return (NULL);
}

t_itemstack			*create_itemstack_from_type(t_doom *doom, t_item_type type,
	t_weapon_type weapon_type)
{
	t_itemstack	*it;
	t_item		*item;

	item = create_item_from_type(doom, type, weapon_type);
	if (!item || !(it = create_itemstack(item, 1)))
		return (NULL);
	return (it);
}
