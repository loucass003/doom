/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/07 23:51:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void				update_player_camera(t_player *player)
{
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->entity.position;
	camera->rotation = player->entity.rotation;
	// camera->pos.y += 10;
	// camera->pos.z += 1300;
	// camera->pos.x -= 700;
	// // camera->pos.x = M_PI_2;
	// player->aabb = (t_collide_aabb) {
	// 	.min = ft_vec3_sub(camera->pos, (t_vec3){ 0.2, 0.2, 0.2 }),
	// 	.max = ft_vec3_add(camera->pos, (t_vec3){ 0.2, 0.2, 0.2 })
	// };
	camera_update_maxtrix(&player->camera);
}
