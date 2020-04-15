/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_item_weapon.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 14:43:37 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/15 17:10:43 by Lisa             ###   ########.fr       */
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

void		set_current_animation_step(t_weapon *weapon, int i)
{
	const int		row = i / (int)weapon->cells.x;
	t_vec2			cell_size;

	cell_size = (t_vec2){
		weapon->bounds.w / (int)weapon->cells.x,
		weapon->bounds.h / (int)weapon->cells.y };
	weapon->curr_image.y = weapon->bounds.y + (int)cell_size.y * row;
	weapon->curr_image.x = weapon->bounds.x + (int)cell_size.x * (i - (row
		* (int)weapon->cells.x));
	weapon->curr_image.w = cell_size.x;
	weapon->curr_image.h = cell_size.y;
}

t_item		*create_item_weapon_gun(t_ressource *image, t_ressource *animation)
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

t_item		*create_item_weapon_axe(t_ressource *image, t_ressource *animation)
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

void		on_use_grenada(t_doom *d, t_itemstack *is)
{
	t_weapon		*weapon;
	t_renderable	g;
	t_vec3			forward;

	weapon = &is->of->data.weapon;
	if (is->amount > 0)
	{
		copy_renderable(d->res_manager.ressources->values[7]->data.model, &g);
		create_grenada(&g, d);
		g.of.data.entity->position = d->player.entity.position;
		forward = vec3_rotate((t_vec3){ 0, 0, 1 }, (t_vec3){
			-d->player.entity.rotation.x, d->player.entity.rotation.y, 0});
		forward.y *= 20;
		forward.x *= 18;
		forward.z *= 18;
		g.of.data.entity->velocity = forward;
		g.of.data.entity->of.grenada.start = SDL_GetTicks();
		append_renderables_array(&d->renderables, g);
		player_sound(&d->audio, CHAR_SHOOTING, 1, 1.5);//TROUVER UN SON POUR LANCER DE GRENADE
		is->amount--;
	}
	if (is->amount <= 0)
		is->of = NULL;
}

t_item		*create_item_weapon_grenada(t_ressource *image)
{
	t_item					*grenada;

	if (!(grenada = create_item_weapon(image, (SDL_Rect){317, 1, 250, 255},
		WEAPON_GRENADA, 75)))
		return (NULL);
	grenada->on_use = on_use_grenada;
	return (grenada);
}
