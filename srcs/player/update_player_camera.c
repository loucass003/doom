/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player_camera.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/12 13:06:43 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/12 15:35:01 by Lisa             ###   ########.fr       */
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
	alListenerfv(AL_ORIENTATION, (ALfloat[6]){camera->forward.x,
		camera->forward.y, camera->forward.z, 0.f, 1.f, 0.f});
	player->entity.packet.doom->lights->values[0].position = player->camera.pos;
	player->entity.packet.doom->lights->values[0].dir = player->camera.forward;
	doom = player->entity.packet.doom;
	update_trigger_area(doom, player);
}
