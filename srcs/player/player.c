/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 17:53:53 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"
#include "script.h"

void				update_trigger_area(t_doom *doom, t_player *player)
{
	t_script_data	*s_data;
	t_script		*script;
	float			dist;
	int				i;

	if (doom->main_context.type == CTX_NORMAL)
	{
		i = -1;
		s_data = doom->res_manager.ressources->values[26]->data.script_data;
		while (++i < s_data->script_count)
		{
			script = &s_data->scripts[i];
			dist = ft_vec3_len(ft_vec3_sub(player->entity.position,
				script->trigger.data.area.pos));
			if (script->trigger.type == TRIG_AREA
				&& dist <= script->trigger.data.area.radius)
				trigger_event(doom, script->trigger);
		}
	}
}

void				update_player_camera(t_player *player)
{
	t_doom			*doom;
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->entity.position;
	camera->rotation = player->entity.rotation;
	camera_update_maxtrix(&player->camera);
	alListener3f(AL_POSITION, player->camera.pos.x, player->camera.pos.y,
		player->camera.pos.z);
	alListenerfv(AL_ORIENTATION, (float[6]){camera->forward.x,
		camera->forward.y, camera->forward.z, 0.f, 1.f, 0.f});
	player->entity.packet.doom->lights->values[0].position = player->camera.pos;
	player->entity.packet.doom->lights->values[0].dir = player->camera.forward;
	doom = player->entity.packet.doom;
	update_trigger_area(doom, player);
}

t_bool				create_player(t_renderable *r, t_doom *doom)
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
	t_vec3	pos;

	pos = editor_to_world(doom->player.spawn_data.position);
	doom->player.entity.position = pos;
	doom->player.entity.position.y += doom->player.entity.radius.y + 0.1;
	doom->player.entity.rotation = doom->player.spawn_data.rotation;
	if (doom->main_context.type == CTX_NORMAL)
		trigger_event(doom, (t_trigger){.type = TRIG_SPAWN});
}
