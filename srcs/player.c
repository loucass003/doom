/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 02:17:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void				update_player_camera(t_player *player)
{
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->entity.position;
	//camera->pos.y += 1;
	camera->rotation = player->entity.rotation;
	camera_update_maxtrix(&player->camera);
}
