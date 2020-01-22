/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:16:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 11:12:18 by louali           ###   ########.fr       */
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


t_item	*create_item(t_item_type type, t_ressource *image, SDL_Rect bounds, int max_stack_size)
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
	
	weapon->curr_image.y = weapon->bounds.y + (int)cell_size.y * row;
	weapon->curr_image.x = weapon->bounds.x + (int)cell_size.x * (i - (row * (int)weapon->cells.x));
	weapon->curr_image.w = cell_size.x;
	weapon->curr_image.h = cell_size.y;
}

void	on_use_weapon(t_doom *doom, t_itemstack *is)
{
	t_weapon	*weapon;
	t_itemstack	*inv_is;
	t_collision	hit;
	int			slot;

	weapon = &is->of->data.weapon;
	if (weapon->fireing || (slot = get_slot_of(&doom->player, ITEM_AMMO)) == -1)
		return ;
	inv_is = &doom->player.item[slot];
	if (inv_is->amount > 0)
	{
		weapon->fireing = TRUE;
		// play_music(&doom->audio, (t_vec3){0, 0, 0}, 7, TRUE);
		player_sound(&doom->audio, CHAR_SHOOTING, 1, 1.5);
		inv_is->amount--;
		doom->gameover.bullets += 1;
	}
	if (inv_is->amount <= 0)
		inv_is->of = NULL;
	
	hit = ray_hit_world(doom, doom->renderables, create_shoot_ray(doom->player, (t_vec3){0, 0, 1}));
	if (hit.collide && hit.renderable->of.type == RENDERABLE_ENTITY && (hit.renderable->of.data.entity->type == ENTITY_ENEMY || hit.renderable->of.data.entity->type == ENTITY_BOSS))
	{
		give_damage(&doom->player.entity, hit.renderable->of.data.entity, doom, weapon->damage);
		hit.renderable->of.data.entity->of.enemy.focus = TRUE;
	}
}

void	on_use_axe(t_doom *doom, t_itemstack *is)
{
	t_collision	hit;
	t_weapon	*weapon;

	weapon = &is->of->data.weapon;
	if (weapon->fireing)
		return;
	weapon->fireing = TRUE;
	hit = ray_hit_world(doom, doom->renderables, create_shoot_ray(doom->player, (t_vec3){0, 0, 1}));
	if (hit.collide && hit.dist < 2 && hit.renderable->of.type == RENDERABLE_ENTITY && (hit.renderable->of.data.entity->type == ENTITY_ENEMY || hit.renderable->of.data.entity->type == ENTITY_BOSS))
	{
		give_damage(&doom->player.entity, hit.renderable->of.data.entity, doom, weapon->damage);
		hit.renderable->of.data.entity->of.enemy.focus = TRUE;
	}
}

t_item	*create_item_weapon_gun(t_ressource *image, t_ressource *animation)
{
	static const uint8_t	seq[6] = { 0, 1, 2, 3, 4, 5 };
	t_item					*gun;

	if (!(gun = create_item_weapon(image, (SDL_Rect){1, 259, 123, 97}, WEAPON_GUN)))
		return (NULL);
	gun->data.weapon.animation = animation;
	gun->data.weapon.bounds = (SDL_Rect){ 0, 0, animation->data.texture->width, animation->data.texture->height - 120 };
	gun->data.weapon.cells = (t_vec2){ 3, 6 };
	gun->data.weapon.animation_seq = (uint8_t *)seq;
	gun->data.weapon.steps_count = 6;
	gun->on_use = on_use_weapon;
	set_current_animation_step(&gun->data.weapon, 0);
	return (gun);
}

t_item	*create_item_weapon_axe(t_ressource *image, t_ressource *animation)
{
	static const uint8_t	seq[7] = { 3, 2, 1, 0, 1, 2, 3 };
	t_item					*axe;

	if (!(axe = create_item_weapon(image, (SDL_Rect){257, 1, 60, 136}, WEAPON_AXE)))
		return (NULL);
	axe->data.weapon.animation = animation;
	axe->data.weapon.bounds = (SDL_Rect){ 0, 194, 439 * 2, 601 * 2 };
	axe->data.weapon.cells = (t_vec2){ 2, 2 };
	axe->data.weapon.animation_seq = (uint8_t *)seq;
	axe->data.weapon.steps_count = 7;
	axe->on_use = on_use_axe;
	set_current_animation_step(&axe->data.weapon, 3);
	return (axe);
}

t_item	*create_item_weapon(t_ressource *image, SDL_Rect bounds, t_weapon_type type)
{
	t_item	*weapon;

	if (!(weapon = create_item(ITEM_WEAPON, image, bounds, 1)))
		return (NULL); //TODO: FREE
	weapon->data.weapon.type = type;
	weapon->data.weapon.damage = 0.6;
	return (weapon);
}

t_item	*create_item_jetpack(t_ressource *image)
{
	t_item	*jetpack;
	
	if (!(jetpack = create_item(ITEM_JETPACK, image, (SDL_Rect){ 257, 257, 146, 103 }, INT_MAX)))
		return (NULL);
	return (jetpack);
}

t_item	*create_item_ammo(t_ressource *image)
{
	t_item	*ammo;
	
	if (!(ammo = create_item(ITEM_AMMO, image, (SDL_Rect){ 1, 1, 256, 256 }, 75)))
		return (NULL);
	return (ammo);
}

t_item	*create_item_heal(t_ressource *image)
{
	t_item	*heal;
	
	if (!(heal = create_item(ITEM_HEAL, image, (SDL_Rect){ 126, 259, 100, 100 }, 1)))
		return (NULL);
	return (heal);
}

t_itemstack	*create_itemstack(t_item *item, int amount)
{
	t_itemstack	*is;

	if (!(is = ft_memalloc(sizeof(t_itemstack))))
		return (NULL);
	is->of = item;
	is->amount = amount;
	return (is);
}

t_item		*create_item_weapon_from_type(t_doom *doom, t_weapon_type type)
{
	if (type == WEAPON_GUN)
		return (create_item_weapon_gun(doom->res_manager.ressources->values[2], doom->res_manager.ressources->values[3]));
	else if (type == WEAPON_AXE)
		return (create_item_weapon_axe(doom->res_manager.ressources->values[2], doom->res_manager.ressources->values[4]));
	return (NULL);
}

t_item			*create_item_from_type(t_doom *doom, t_item_type type, t_weapon_type weapon_type)
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

t_itemstack		*create_itemstack_from_type(t_doom *doom, t_item_type type, t_weapon_type weapon_type)
{
	t_itemstack	*it;
	t_item	*item;

	item = create_item_from_type(doom, type, weapon_type);
	if (!item || !(it = create_itemstack(item, 1)))
		return (NULL);
	return (it);
}

t_bool	create_itemstack_renderable(t_renderable *r, t_item *item, int amount)
{
	t_sprite	*sprite;

	if (!(sprite = create_sprite((t_vec2){ 1, 1 }, item->image)))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	r->of.type = RENDERABLE_ITEMSTACK;
	if (!(r->of.data.itemstack = create_itemstack(item, amount)))
		return (FALSE);
	sprite = r->sprite;
	sprite->uv_max = (t_vec2){ (float)item->bounds.x / (float)item->image->data.texture->width, 1 - (float)item->bounds.y / (float)item->image->data.texture->height };
	sprite->uv_min = (t_vec2){ (float)(item->bounds.x + item->bounds.w) / (float)item->image->data.texture->width, 1 - (float)(item->bounds.y + item->bounds.h) / (float)item->image->data.texture->height };
	r->vertex->vertices[0] = sprite->uv_min;
	r->vertex->vertices[1] = (t_vec2){ sprite->uv_max.x, sprite->uv_min.y };
	r->vertex->vertices[2] = sprite->uv_max;
	r->vertex->vertices[3] = (t_vec2){ sprite->uv_min.x, sprite->uv_max.y };
	r->scale = (t_vec3){ 1.5, 1.5, 1.5 };
	r->object_index = -1;
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
		entity->life = fmin(entity->max_life, entity->life + (0.5 * entity->packet.doom->level.coeff_regen));
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
		if (inv_is->of->type == ITEM_WEAPON)
			entity->packet.doom->gameover.weapon += ft_min(i, is->amount);
		inv_is->amount += ft_min(i, is->amount);
		is->amount -= ft_min(i, is->amount);
		player_sound(&entity->packet.doom->audio, ITEM_PICK, 3, 1);
	}
	return (TRUE);
}
