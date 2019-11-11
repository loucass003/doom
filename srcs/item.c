/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:16:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/10 23:40:15 by llelievr         ###   ########.fr       */
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

void	set_current_animation_step(t_weapon *weapon, int i)
{
	const t_vec2	cell_size = (t_vec2){ weapon->bounds.w / (int)weapon->cells.x,
		weapon->bounds.h / (int)weapon->cells.y };
	const int		row = i / (int)weapon->cells.x;
	
	weapon->curr_image.y = (int)cell_size.y * row;
	weapon->curr_image.x = (int)cell_size.x * (i - (row * (int)weapon->cells.x));
	weapon->curr_image.w = cell_size.x;
	weapon->curr_image.h = cell_size.y;
}

void	on_use_weapon(t_doom *doom, t_itemstack *is)
{
	t_weapon	*weapon;
	t_itemstack	*inv_is;
	t_collision	hit;
	int			slot;
	int			i;

	weapon = &is->of->data.weapon;
	if (weapon->fireing || (slot = get_slot_of(&doom->player, ITEM_AMMO)) == -1)
		return ;
	inv_is = &doom->player.item[slot];
	if (inv_is->amount > 0)
	{
		weapon->fireing = TRUE;
		// play_music(&doom->audio, doom->player.camera.pos, 7);
		play_music(&doom->audio, (t_vec3){0, 0, 0}, 7, TRUE);
		inv_is->amount--;
	}
	if (inv_is->amount <= 0)
		inv_is->of = NULL;
	
	hit = ray_hit_world(doom, doom->renderables, create_shoot_ray(doom->player, (t_vec3){0, 0, 1}));
	if (hit.collide && hit.renderable->of.type == RENDERABLE_ENTITY && hit.renderable->of.data.entity->type == ENTITY_ENEMY)
	{
		hit.renderable->of.data.entity->life -= weapon->damage;
		hit.renderable->of.data.entity->focus = TRUE;
	}
}

t_item	*create_item_weapon_gun(SDL_Surface *image, SDL_Surface *animation)
{
	static const uint8_t	seq[6] = { 0, 1, 2, 3, 4, 5 };
	t_item					*gun;

	if (!(gun = create_item_weapon(image, (SDL_Rect){0, 768, 120, 120}, WEAPON_GUN)))
		return (NULL);
	gun->data.weapon.animation = animation;
	gun->data.weapon.bounds = (SDL_Rect){ 0, 0, animation->w, animation->h - 120 };
	gun->data.weapon.cells = (t_vec2){ 3, 6 };
	gun->data.weapon.animation_seq = (uint8_t *)seq;
	gun->data.weapon.steps_count = 6;
	gun->on_use = on_use_weapon;
	set_current_animation_step(&gun->data.weapon, 0);
	return (gun);
}

t_item	*create_item_weapon(SDL_Surface *image, SDL_Rect bounds, t_weapon_type type)
{
	t_item	*weapon;

	if (!(weapon = create_item(ITEM_WEAPON, image, bounds, 1)))
		return (NULL); //TODO: FREE
	weapon->data.weapon.type = type;
	weapon->data.weapon.damage = 0.6;
	return (weapon);
}

t_item	*create_item_ammo(SDL_Surface *image)
{
	t_item	*ammo;
	
	if (!(ammo = create_item(ITEM_AMMO, image, (SDL_Rect){ 0, 0, image->w, image->h }, 75)))
		return (NULL);
	return (ammo);
}

t_item	*create_item_heal(SDL_Surface *image)
{
	t_item	*heal;
	
	if (!(heal = create_item(ITEM_HEAL, image, (SDL_Rect){ 0, 0, image->w, image->h }, 1)))
		return (NULL);
	return (heal);
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
		entity->life = fmin(entity->max_life, entity->life + 0.5);
		return (TRUE);
	}
	s = get_slot(&entity->packet.doom->player, is);
	if (s == -1 && (s = get_empty_slot(&entity->packet.doom->player)) == -1)
		return (TRUE);
	inv_is = &entity->packet.doom->player.item[s];
	if (inv_is->of && inv_is->of->type == ITEM_WEAPON 
		&& inv_is->of->data.weapon.type == is->of->data.weapon.type)
		return (TRUE);
	else if (inv_is->of && inv_is->of->type == ITEM_WEAPON 
		&& (s = get_empty_slot(&entity->packet.doom->player)) == -1)
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
