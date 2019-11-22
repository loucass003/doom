/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:53:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/22 20:30:04 by llelievr         ###   ########.fr       */
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

t_bool		read_item(t_ressource_manager *r, t_item **item)
{
	t_item_type		item_type;
	t_weapon_type	weapon_type;

	item_type = ITEM_UNKNOWN;
	weapon_type = -1;
	if (!io_memcpy(&r->reader, &item_type, sizeof(t_item_type)))
		return (FALSE);
	if (item_type == ITEM_WEAPON && !io_memcpy(&r->reader, &weapon_type, sizeof(t_weapon_type)))
		return (FALSE);
	*item = create_item_from_type(r->doom, item_type, weapon_type);
	if (!*item)
		return (FALSE);
	return (TRUE);
}

t_bool		read_itemstack(t_ressource_manager *r, t_itemstack **is)
{
	int				amount;
	t_item			*item;

	if (!io_memcpy(&r->reader, &amount, sizeof(int)))
		return (FALSE);
	if (!read_item(r, &item))
		return (FALSE);
	if (!(*is = create_itemstack(item, amount)))
		return (FALSE);
	return (TRUE);
}

t_bool		read_object(t_ressource_manager *r, t_object *object)
{
	t_wr_object	wr_object;

	if (!io_memcpy(&r->reader, &wr_object, sizeof(t_wr_object)))
		return (FALSE);
	*object = (t_object) { .type = wr_object.type, .pos = wr_object.pos,
		.scale = wr_object.scale, .no_light = wr_object.no_light };

	if (wr_object.type == OBJECT_ITEMSTACK && !read_itemstack(r, &object->of.itemstack))
		return (FALSE);
	return (TRUE);
}

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

t_bool		read_map(t_ressource_manager *r)
{
	if (!read_points(r))
		return (FALSE);
	if (!read_rooms(r))
		return (FALSE);
	if (!read_objects(r))
		return (FALSE);
	return (TRUE);
}
