/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/12 15:50:43 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"
#include "octree.h"
#include "script.h"

t_bool	create_player(t_renderable *r, t_doom *doom)
{
	if (!create_renderable(r, RENDERABLE_ENTITY))
		return (FALSE);
	r->of.data.entity = &doom->player.entity;
	r->of.data.entity->type = ENTITY_PLAYER;
	r->of.data.entity->packet.doom = doom;
	r->visible = FALSE;
	compute_ellipsoid_hitbox(r, r->of.data.entity->position,
		r->of.data.entity->radius);
	return (TRUE);
}

void				init_player(t_doom *doom)
{
	t_player		*player;

	player = &doom->player;
	player->camera.projection = projection_matrix();
	ft_bzero(player->item, sizeof(t_itemstack) * 8);
	ft_bzero(&doom->gameover, sizeof(t_gameover));
	player->entity.position = (t_vec3){ -30, 50, 0 };
	player->entity.velocity = (t_vec3){ 0, 0, 0 };
	player->entity.radius = (t_vec3){ 1, 2.5f, 1 };
	player->entity.packet.doom = doom;
	player->entity.rotation.y = -M_PI;
	player->entity.rotation.x = 0;
	player->entity.rotation.z = 0;
	player->entity.killable = TRUE;
	player->entity.life = doom->level.max_life;
	player->entity.max_life = doom->level.max_life;
	doom->main_context.camera = &player->camera;
	set_player_state(doom, player, PS_NORMAL);
	update_player_camera(&doom->player);
}

void				spawn_player(t_doom *doom)
{
	doom->player.entity.position = editor_to_world(doom->player
		.spawn_data.position);
	doom->player.entity.position.y += doom->player.entity.radius.y + 0.1;
	doom->player.entity.rotation = doom->player.spawn_data.rotation;
	if (doom->main_context.type == CTX_NORMAL)
		trigger_event(doom, (t_trigger){.type = TRIG_SPAWN});
}

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

	player_inventory_normal(doom, key, event);
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_END))
		{
			doom->player.spawn_data.position = world_to_editor(doom->player\
				.entity.position);
			doom->player.spawn_data.position.y -= doom->player.entity.radius.y;
			doom->player.spawn_data.rotation = doom->player.entity.rotation;
		}
	}
}
