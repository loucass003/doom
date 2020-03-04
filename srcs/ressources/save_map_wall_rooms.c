/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map_wall_rooms.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:42:09 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:42:42 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "item.h"
#include "sprite.h"
#include "write_structs.h"

t_bool		write_wall_sections(t_ressource_manager *r, t_wall_section *ws)
{
	t_wr_wall_section	wr_ws;

	wr_ws = (t_wr_wall_section) {
		.type = ws->type,
		.resource_index = ressources_indexof(r->ressources, ws->texture),
		.normal_type = ws->normal_type,
		.invisible = ws->invisible,
		.collisions = ws->collisions,
		.uv_offset = ws->uv_offset,
		.uv_repeat = ws->uv_repeat
	};
	dp_write(r, &wr_ws, sizeof(t_wr_wall_section));
	return (TRUE);
}

t_bool		write_wall(t_ressource_manager *r, t_wall *wall)
{
	t_wr_wall	wr_wall;
	int			i;

	wr_wall = (t_wr_wall) {
		.indice = wall->indice,
		.floor_height = wall->floor_height,
		.ceiling_height = wall->ceiling_height,
		.wall_sections_count = wall->wall_sections
			? wall->wall_sections->len : 0
	};
	dp_write(r, &wr_wall, sizeof(t_wr_wall));
	i = -1;
	while (++i < wr_wall.wall_sections_count)
		if (!write_wall_sections(r, &wall->wall_sections->values[i]))
			return (FALSE);
	return (TRUE);
}

t_bool		write_room(t_ressource_manager *r, t_room *room)
{
	int				i;
	t_wr_room		wr_room;

	wr_room = (t_wr_room) { .closed = room->closed && room->walls->len >= 3,
		.walls_count = room->walls->len,
		.floor_res_index = ressources_indexof(r->ressources,
			room->floor_texture),
		.ceiling_res_index = ressources_indexof(r->ressources,
			room->ceiling_texture), .floor_rot = room->floor_rot,
		.ceil_rot = room->ceil_rot, .floor_invisible = room->floor_invisible,
		.ceil_invisible = room->ceil_invisible,
		.floor_collision = room->floor_collision,
		.ceil_collision = room->ceil_collision,
		.floor_normal = room->floor_normal, .ceil_normal = room->ceil_normal,
		.ambiant_light = room->ambiant_light,
		.floor_uv_offset = room->floor_uv_offset,
		.floor_uv_repeat = room->floor_uv_repeat,
		.ceil_uv_offset = room->ceil_uv_offset,
		.ceil_uv_repeat = room->ceil_uv_repeat};
	dp_write(r, &wr_room, sizeof(t_wr_room));
	i = -1;
	while (++i < room->walls->len)
		if (!write_wall(r, &room->walls->values[i]))
			return (FALSE);
	return (TRUE);
}

t_bool		write_rooms(t_ressource_manager *r)
{
	int		i;

	dp_write(r, &r->doom->editor.rooms->len, sizeof(int));
	i = -1;
	while (++i < r->doom->editor.rooms->len)
		if (!write_room(r, &r->doom->editor.rooms->values[i]))
			return (FALSE);
	return (TRUE);
}
