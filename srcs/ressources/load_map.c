/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:53:42 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/04 17:26:50 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "sprite.h"

t_bool		read_objects(t_ressource_manager *r)
{
	int			objects_count;
	int			i;
	t_object	o;

	if (!io_memcpy(&r->reader, &objects_count, sizeof(int)))
		return (FALSE);
	i = -1;
	while (++i < objects_count)
	{
		if (!read_object(r, &o))
			return (FALSE);
		if (!append_objects_array(&r->doom->editor.objects, o))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		read_player(t_ressource_manager *r)
{
	t_wr_player wr_player;

	if (!io_memcpy(&r->reader, &wr_player, sizeof(t_wr_player)))
		return (FALSE);
	r->doom->editor.player_set = wr_player.set;
	r->doom->player.spawn_data.position = wr_player.position;
	r->doom->player.spawn_data.rotation = wr_player.rotation;
	return (TRUE);
}

t_bool		read_globals(t_ressource_manager *r)
{
	t_wr_globals wr_globals;

	if (!io_memcpy(&r->reader, &wr_globals, sizeof(t_wr_globals)))
		return (FALSE);
	r->doom->skybox_enabled = wr_globals.skybox;
	return (TRUE);
}

t_bool		read_scores(t_ressource_manager *r)
{
	if (!io_memcpy(&r->reader, &r->doom->scores, sizeof(t_score) * 5))
		return (FALSE);
	return (TRUE);
}

t_bool		read_map(t_ressource_manager *r)
{
	if (!read_globals(r))
		return (FALSE);
	if (!read_points(r))
		return (FALSE);
	if (!read_rooms(r))
		return (FALSE);
	if (!read_objects(r))
		return (FALSE);
	if (!read_player(r))
		return (FALSE);
	if (!read_scores(r))
		return (FALSE);
	return (TRUE);
}
