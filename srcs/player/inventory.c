/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inventory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:23:52 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/16 19:45:00 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

void				player_inventory_normal(t_doom *doom,
	const SDL_Scancode key, SDL_Event *event)
{
	t_itemstack	*is;

	if (doom->main_context.type == CTX_NORMAL)
	{
		if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			is = &doom->player.item[doom->player.selected_slot];
			if (is->of && is->of->on_use)
				is->of->on_use(doom, is);
		}
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_UP
			|| key == SDL_SCANCODE_DOWN))
		{
			doom->player.selected_slot += (key == SDL_SCANCODE_DOWN ? 1 : -1);
			if (doom->player.selected_slot < 0)
				doom->player.selected_slot = 7;
			if (doom->player.selected_slot > 7)
				doom->player.selected_slot = 0;
		}
		if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_C)
			set_player_state(doom, &doom->player,
			doom->player.desired_state == PS_CROUCH ? PS_NORMAL : PS_CROUCH);
	}
}

void				player_inventory_event(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;
	t_vec3				pos;

	player_inventory_normal(doom, key, event);
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_RETURN))
		{
			pos = world_to_editor(doom->player.entity.position);
			doom->player.spawn_data.position = pos;
			doom->player.spawn_data.position.y -= doom->player.entity.radius.y;
			doom->player.spawn_data.rotation = doom->player.entity.rotation;
		}
	}
}
