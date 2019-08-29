/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 20:47:46 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void				update_player_camera(t_player *player)
{
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->pos;
	camera->rotation = player->rotation;
	camera_update_maxtrix(&player->camera);
}
