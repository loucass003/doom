/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_item_weapon.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 14:43:37 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/20 15:50:51 by lloncham         ###   ########.fr       */
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

void	set_current_animation_step(t_weapon *weapon, int i)
{
	const int		row = i / (int)weapon->cells.x;
	const t_vec2	cell_size = (t_vec2){
        weapon->bounds.w / (int)weapon->cells.x,
        weapon->bounds.h / (int)weapon->cells.y };

	weapon->curr_image.y = weapon->bounds.y + (int)cell_size.y * row;
	weapon->curr_image.x = weapon->bounds.x + (int)cell_size.x * (i - (row
		* (int)weapon->cells.x));
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
		player_sound(&doom->audio, CHAR_SHOOTING, 1, 1.5);
		inv_is->amount--;
		doom->gameover.bullets += 1;
	}
	if (inv_is->amount <= 0)
		inv_is->of = NULL;
	hit = ray_hit_world(doom, doom->renderables,
		create_shoot_ray(doom->player, (t_vec3){0, 0, 1}));
	if (hit.collide && hit.renderable->of.type == RENDERABLE_ENTITY
        && (hit.renderable->of.data.entity->type == ENTITY_ENEMY
        || hit.renderable->of.data.entity->type == ENTITY_BOSS))
	{
		give_damage(&doom->player.entity, hit.renderable->of.data.entity,
			doom, weapon->damage);
		hit.renderable->of.data.entity->of.enemy.focus = TRUE;
	}
}

void	on_use_axe(t_doom *doom, t_itemstack *is)
{
	t_collision	hit;
	t_weapon	*weapon;

	weapon = &is->of->data.weapon;
	if (weapon->fireing)
		return ;
	weapon->fireing = TRUE;
	hit = ray_hit_world(doom, doom->renderables, create_shoot_ray(doom->player,
		(t_vec3){0, 0, 1}));
	if (hit.collide && hit.dist < 2
		&& hit.renderable->of.type == RENDERABLE_ENTITY
        && (hit.renderable->of.data.entity->type == ENTITY_ENEMY
        || hit.renderable->of.data.entity->type == ENTITY_BOSS))
	{
		give_damage(&doom->player.entity, hit.renderable->of.data.entity, doom,
			weapon->damage);
		hit.renderable->of.data.entity->of.enemy.focus = TRUE;
	}
}

t_item	*create_item_weapon_gun(t_ressource *image, t_ressource *animation)
{
	static const uint8_t	seq[6] = { 0, 1, 2, 3, 4, 5 };
	t_item					*gun;

	if (!(gun = create_item_weapon(image, (SDL_Rect){1, 259, 123, 97},
		WEAPON_GUN, 1)))
		return (NULL);
	gun->data.weapon.animation = animation;
	gun->data.weapon.bounds = (SDL_Rect){ 0, 0, animation->data.texture->width,
		animation->data.texture->height - 120 };
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

	if (!(axe = create_item_weapon(image, (SDL_Rect){257, 1, 60, 136},
		WEAPON_AXE, 1)))
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

void	on_use_grenada(t_doom *doom, t_itemstack *is)
{
	t_weapon	*weapon;

	weapon = &is->of->data.weapon;
	if (is->amount > 0)
	{
		t_renderable grenada;
		t_vec3 forward;
		grenada = *doom->res_manager.ressources->values[7]->data.model;
		create_grenada(&grenada, doom);
		grenada.of.data.entity->position = doom->player.entity.position;
		forward = vec3_rotate((t_vec3){ 0, 0, 1 }, (t_vec3){-doom
			->player.entity.rotation.x, doom->player.entity.rotation.y, 0});
		forward.y *= 20;
		forward.x *= 18;
		forward.z *= 18;
		grenada.of.data.entity->velocity = forward;
		grenada.of.data.entity->of.grenada.start = SDL_GetTicks();
		append_renderables_array(&doom->renderables, grenada);
		player_sound(&doom->audio, CHAR_SHOOTING, 1, 1.5);//TROUVER UN SON POUR LANCER DE GRENADE
		is->amount--;
	}
	if (is->amount <= 0)
		is->of = NULL;

}

t_item	*create_item_weapon_grenada(t_ressource *image)
{
	t_item					*grenada;

	if (!(grenada = create_item_weapon(image, (SDL_Rect){317, 1, 250, 255},
		WEAPON_GRENADA, 75)))
		return (NULL);
	grenada->on_use = on_use_grenada;
	return (grenada);
}