/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:16:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/12 03:24:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include "doom.h"
#include "item.h"

t_item	*create_item(t_item_type type, SDL_Surface *image, SDL_Rect bounds, int max_stack_size)
{
	t_item	*item;

	if (!(item = ft_memalloc(sizeof(t_item))))
		return (NULL);
	item->type = type;
	item->image = image;
	item->bounds = bounds;
	item->max_stack_size = max_stack_size;
	return (item);
}

t_item	*create_item_weapon(SDL_Surface *image, SDL_Rect bounds, t_weapon_type type)
{
	t_item	*weapon;

	if (!(weapon = create_item(ITEM_WEAPON, image, bounds, 1)))
		return (NULL); //TODO: FREE
	weapon->data.weapon.type = type;
	weapon->data.weapon.damage = 0.3;
	return (weapon);
}

t_bool	create_itemstack_renderable(t_renderable *r, t_item *item, int amount)
{
	t_sprite	*sprite;

	if (!create_sprite(r, (t_mtl){ .texture_map = item->image, .texture_map_set = TRUE }, (t_vec2){ 1, 1 }))
		return (FALSE);
	r->of.type = RENDERABLE_ITEMSTACK;
	if (!(r->of.data.itemstack = ft_memalloc(sizeof(t_itemstack))))
		return (FALSE);
	r->of.data.itemstack->of = item;
	r->of.data.itemstack->amount = amount;
	sprite = r->sprite;
	sprite->uv_max = (t_vec2){ (float)item->bounds.x / (float)item->image->w, 1 - (float)item->bounds.y / (float)item->image->h };
	sprite->uv_min = (t_vec2){ (float)(item->bounds.x + item->bounds.w) / (float)item->image->w, 1 - (float)(item->bounds.y + item->bounds.h) / (float)item->image->h };
	r->vertex->vertices[0] = sprite->uv_min;
	r->vertex->vertices[1] = (t_vec2){ sprite->uv_max.x, sprite->uv_min.y };
	r->vertex->vertices[2] = sprite->uv_max;
	r->vertex->vertices[3] = (t_vec2){ sprite->uv_min.x, sprite->uv_max.y };
	r->scale = (t_vec3){ 1.5, 1.5, 1.5 };
	sprite->hitbox_radius = ft_vec3_mul_s(r->scale, 0.5);
	//r->show_hitbox = TRUE;
	compute_sprite_hitbox(r);
	return (TRUE);
}

int					get_slot(t_player *player, t_itemstack *is)
{
	int		i;

	i = -1;
	while (++i < PLAYER_INV_SIZE)
		if (player->item[i].of && player->item[i].of->type == is->of->type)
			return (i);
	return (-1);
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
	s = get_slot(&entity->packet.doom->player, is);
	if (s == -1 && (s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (TRUE);
	inv_is = &entity->packet.doom->player.item[s];
	if (inv_is->of && inv_is->of->type == ITEM_WEAPON 
		&& inv_is->of->data.weapon.type == is->of->data.weapon.type)
		return (TRUE);
	else if ((s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (TRUE); 
	inv_is = &entity->packet.doom->player.item[s];
	inv_is->of = is->of;
	i = inv_is->of->max_stack_size - inv_is->amount;
	if (i > 0)
	{
		inv_is->amount += ft_min(i, is->amount);
		is->amount -= ft_min(i, is->amount);
	}
	return (TRUE);
}