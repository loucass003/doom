/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itemstack_renderable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 14:51:01 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/07 03:21:46 by llelievr         ###   ########.fr       */
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

t_itemstack		*create_itemstack(t_item *item, int amount)
{
	t_itemstack	*is;

	if (!(is = ft_memalloc(sizeof(t_itemstack))))
		return (NULL);
	is->of = item;
	is->amount = amount;
	return (is);
}

t_item			*create_item_weapon_from_type(t_doom *doom, t_weapon_type type)
{
	if (type == WEAPON_GUN)
		return (create_item_weapon_gun(doom->res_manager.ressources->values[2],
		doom->res_manager.ressources->values[3]));
	else if (type == WEAPON_AXE)
		return (create_item_weapon_axe(doom->res_manager.ressources->values[2],
			doom->res_manager.ressources->values[4]));
	else if (type == WEAPON_GRENADA)
		return (create_item_weapon_grenada(doom->res_manager.ressources
			->values[2]));
	return (NULL);
}

t_item			*create_item_from_type(t_doom *doom, t_item_type type,
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

t_itemstack		*create_itemstack_from_type(t_doom *doom, t_item_type type,
	t_weapon_type weapon_type)
{
	t_itemstack	*it;
	t_item		*item;

	item = create_item_from_type(doom, type, weapon_type);
	if (!item || !(it = create_itemstack(item, 1)))
		return (NULL);
	return (it);
}

t_bool			create_itemstack_renderable(t_renderable *r, t_itemstack *is)
{
	t_sprite	*sprite;
	t_item		*item;

	item = is->of;
	if (!(sprite = create_sprite((t_vec2){ 1, 1 }, is->of->image)))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	r->of.type = RENDERABLE_ITEMSTACK;
	r->of.data.itemstack = is;
	sprite = r->sprite;
	sprite->uv_max = (t_vec2){ (float)item->bounds.x
		/ (float)item->image->data.texture->width,
		1 - (float)item->bounds.y / (float)item->image->data.texture->height };
	sprite->uv_min = (t_vec2){ (float)(item->bounds.x + item->bounds.w)
		/ (float)item->image->data.texture->width,
		1 - (float)(item->bounds.y + item->bounds.h)
		/ (float)item->image->data.texture->height };
	r->vertex->vertices[0] = sprite->uv_min;
	r->vertex->vertices[1] = (t_vec2){ sprite->uv_max.x, sprite->uv_min.y };
	r->vertex->vertices[2] = sprite->uv_max;
	r->vertex->vertices[3] = (t_vec2){ sprite->uv_min.x, sprite->uv_max.y };
	r->scale = (t_vec3){ 1.5, 1.5, 1.5 };
	r->object_index = -1;
	sprite->hitbox_radius = ft_vec3_mul_s(r->scale, 0.5);
	compute_sprite_hitbox(r);
	return (TRUE);
}


void	free_itemstack(t_itemstack **it)
{
	if (!(*it))
		return ;
	ft_memdel((void **)&(*it)->of);
	ft_memdel((void **)it);
}
