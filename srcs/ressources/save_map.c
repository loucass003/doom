/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:20:07 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/04 17:44:47 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "item.h"
#include "sprite.h"
#include "write_structs.h"

t_bool		write_player(t_ressource_manager *r)
{
	t_wr_player	wr_player;

	wr_player = (t_wr_player){
		.set = r->doom->editor.player_set,
		.position = r->doom->player.spawn_data.position,
		.rotation = r->doom->player.spawn_data.rotation
	};
	dp_write(r, &wr_player, sizeof(t_wr_player));
	return (TRUE);
}

t_bool		write_globals(t_ressource_manager *r)
{
	t_wr_globals	wr_globals;

	wr_globals = (t_wr_globals){.skybox = r->doom->skybox_enabled};
	dp_write(r, &wr_globals, sizeof(t_wr_globals));
	return (TRUE);
}

t_bool		write_scores(t_ressource_manager *r)
{
	dp_write(r, r->doom->scores, sizeof(t_score) * 5);
	return (TRUE);
}

t_bool		write_map(t_ressource_manager *r)
{
	write_globals(r);
	write_points(r);
	write_rooms(r);
	write_objects(r);
	write_player(r);
	write_scores(r);
	return (TRUE);
}
