/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/15 04:43:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void				update_player_camera(t_player *player)
{
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->pos;
	camera->rotation = player->rotation;
	player->aabb = (t_collide_aabb) {
		.min = ft_vec3_sub(player->pos, (t_vec3){ 0.2, 0.2, 0.2 }),
		.max = ft_vec3_add(player->pos, (t_vec3){ 0.2, 0.2, 0.2 })
	};
	camera_update_maxtrix(&player->camera);
}
