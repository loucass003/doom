/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/20 13:59:56 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

void				update_player_camera(t_player *player)
{
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->entity.position;
	//camera->pos.y += 1;
	camera->rotation = player->entity.rotation;
	camera_update_maxtrix(&player->camera);

	t_vec3	forward = ft_mat4_mulv(ft_mat4_rotation(player->camera.rotation), (t_vec3){ 0, 0, 1 });
	alListener3f(AL_POSITION, player->camera.pos.x, player->camera.pos.y, player->camera.pos.z);
	alListenerfv(AL_ORIENTATION, (ALfloat[6]){forward.x, forward.y, forward.z, 0.f, 1.f, 0.f});
	// ALenum error = alGetError();
	// 	printf("alGenBuffers : %s\n", alGetString(error)); 
}


void				init_player(t_doom *doom)
{
	t_player		*player;

	player = &doom->player;
	player->camera.projection = projection_matrix();
	// player->entity.position = (t_vec3){ 0, 2.512500, 45.176151 };
	player->entity.position = (t_vec3){ 0, 20, 0 };
	player->entity.velocity = (t_vec3){ 0, 0, 0 };
	player->entity.radius = (t_vec3){ 1, 2.5f, 1 };
	player->entity.packet.doom = doom;
	player->entity.rotation.y = 0;
	player->entity.rotation.x = 0;
	player->entity.rotation.z = 0;
	player->entity.life = 1.5;
	player->entity.max_life = 2;
	doom->main_context.camera = &player->camera;
	update_player_camera(&doom->player);
}