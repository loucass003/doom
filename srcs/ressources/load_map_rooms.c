/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_rooms.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:23:12 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/25 15:54:17 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "sprite.h"

t_room		init_room(t_ressource_manager *r, t_wr_room wr_room, t_walls *walls)
{
	t_room room;

	room = (t_room) { .closed = wr_room.closed, .walls = walls,
		.floor_texture = r->ressources->values[wr_room.floor_res_index],
		.ceiling_texture = r->ressources->values[wr_room.ceiling_res_index],
		.floor_rot = wr_room.floor_rot,
		.ceil_rot = wr_room.ceil_rot,
		.floor_invisible = wr_room.floor_invisible,
		.ceil_invisible = wr_room.ceil_invisible,
		.floor_collision = wr_room.floor_collision,
		.ceil_collision = wr_room.ceil_collision,
		.floor_normal = wr_room.floor_normal,
		.ceil_normal = wr_room.ceil_normal,
		.ambiant_light = wr_room.ambiant_light,
		.floor_uv_repeat = wr_room.floor_uv_repeat,
		.floor_uv_offset = wr_room.floor_uv_offset,
		.ceil_uv_repeat = wr_room.ceil_uv_repeat,
		.ceil_uv_offset = wr_room.ceil_uv_offset,
	};
	return (room);
}

t_bool		check_floor_ceiling_room(t_wr_room wr_room, t_ressource_manager *r)
{
	if (wr_room.ceiling_res_index < 0
		|| wr_room.ceiling_res_index >= r->ressources->len)
		return (FALSE);
	if (wr_room.floor_res_index < 0
		|| wr_room.floor_res_index >= r->ressources->len)
		return (FALSE);
	return (TRUE);
}

t_bool		read_room(t_ressource_manager *r)
{
	t_wr_room	wr_room;
	t_room		room;
	t_walls		*walls;
	int			i;

	if (!io_memcpy(&r->reader, &wr_room, sizeof(t_wr_room)))
		return (FALSE);
	if (wr_room.walls_count < 0)
		return (FALSE);
	if (!(walls = create_walls_array(wr_room.walls_count + 5)))
		return (FALSE);
	if (check_floor_ceiling_room(wr_room, r) == FALSE)
		return (FALSE);
	room = init_room(r, wr_room, walls);
	i = -1;
	while (++i < wr_room.walls_count)
		if (!read_wall(r, &walls))
			return (FALSE);
	if (!append_rooms_array(&r->doom->editor.rooms, room))
		return (FALSE);
	return (TRUE);
}

t_bool		read_rooms(t_ressource_manager *r)
{
	int		rooms_count;
	int		i;

	if (!io_memcpy(&r->reader, &rooms_count, sizeof(int)))
		return (FALSE);
	if (rooms_count < 0)
		return (FALSE);
	i = -1;
	while (++i < rooms_count)
		if (!read_room(r))
			return (FALSE);
	return (TRUE);
}
