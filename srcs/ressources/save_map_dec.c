/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map_dec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:44:39 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:44:55 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "item.h"
#include "sprite.h"
#include "write_structs.h"

t_bool		write_points(t_ressource_manager *r)
{
	dp_write(r, &r->doom->editor.points->len, sizeof(int));
	dp_write(r, &r->doom->editor.points->vertices,
		sizeof(t_vec2) * r->doom->editor.points->len);
	return (TRUE);
}

t_bool		write_item(t_ressource_manager *r, t_item *item)
{
	dp_write(r, &item->type, sizeof(t_item_type));
	if (item->type == ITEM_WEAPON)
		dp_write(r, &item->data.weapon.type, sizeof(t_weapon_type));
	return (TRUE);
}

t_bool		write_itemstack(t_ressource_manager *r, t_itemstack *is)
{
	dp_write(r, &is->amount, sizeof(int));
	write_item(r, is->of);
	return (TRUE);
}

t_bool		write_sprite(t_ressource_manager *r, t_sprite *sprite)
{
	t_wr_sprite	wr_sprite;

	wr_sprite = (t_wr_sprite) {
		.always_facing_player = sprite->always_facing_player,
		.texture_index = ressources_indexof(r->ressources, sprite->texture),
		.hitbox_radius = sprite->hitbox_radius
	};
	dp_write(r, &wr_sprite, sizeof(t_wr_sprite));
	return (TRUE);
}

t_bool		write_entity(t_ressource_manager *r, t_entity_type entity_type)
{
	dp_write(r, &entity_type, sizeof(t_entity_type));
	return (TRUE);
}
