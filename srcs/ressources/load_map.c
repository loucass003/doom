/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:53:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/14 12:54:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"

t_bool		read_wall(t_ressource_manager *r, t_walls *walls)
{
	t_wr_wall		wr_wall;
	t_wall			wall;
	t_ressource	*texture;

	if (!io_memcpy(&r->reader, &wr_wall, sizeof(t_wr_wall)))
		return (FALSE);
	if (wr_wall.resource_index < 0 || wr_wall.resource_index >= r->ressources->len)
		return (FALSE);
	texture = r->ressources->values[wr_wall.resource_index];
	if (texture->type != RESSOURCE_TEXTURE)
		return (FALSE);
	wall = (t_wall) {
		.indice = wr_wall.indice,
		.texture = r->ressources->values[wr_wall.resource_index],
		.normal_type = wr_wall.normal_type,
		.invisible = wr_wall.invisible,
		.collisions = wr_wall.collisions
	};
	if (!append_walls_array(&walls, wall))
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
	if (wr_room.ceiling_res_index < 0 || wr_room.ceiling_res_index >= r->ressources->len)
		return (FALSE);
	if (wr_room.floor_res_index < 0 || wr_room.floor_res_index >= r->ressources->len)
		return (FALSE);
	room = (t_room) { .closed = wr_room.closed, .walls = walls, 
		.floor_texture = r->ressources->values[wr_room.floor_res_index],
		.ceiling_texture = r->ressources->values[wr_room.ceiling_res_index] };
	i = -1;
	while (++i < wr_room.walls_count)
		if (!read_wall(r, walls))
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

t_bool		read_points(t_ressource_manager *r)
{
	int		points_count;
	int		i;
	t_vec2	p;

	if (!io_memcpy(&r->reader, &points_count, sizeof(int)))
		return (FALSE);
	i = -1;
	while (++i < points_count)
	{
		if (!io_memcpy(&r->reader, &p, sizeof(t_vec2)))
			return (FALSE);
		if (!append_2dvertices_array(&r->doom->editor.points, p))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		read_map(t_ressource_manager *r)
{
	if (!read_points(r))
		return (FALSE);
	if (!read_rooms(r))
		return (FALSE);
	return (TRUE);
}
