/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_all.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:26:40 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:27:22 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "sprite.h"

t_bool		read_wall_section(t_ressource_manager *r,
	t_wall_sections **sections)
{
	t_wr_wall_section	wr_ws;
	t_ressource			*texture;
	t_wall_section		ws;

	if (!io_memcpy(&r->reader, &wr_ws, sizeof(t_wr_wall_section)))
		return (FALSE);
	if (wr_ws.resource_index < 0 || wr_ws.resource_index >= r->ressources->len)
		return (FALSE);
	texture = r->ressources->values[wr_ws.resource_index];
	if (texture->type != RESSOURCE_TEXTURE)
		return (FALSE);
	ws = (t_wall_section) {
		.type = wr_ws.type,
		.texture = r->ressources->values[wr_ws.resource_index],
		.normal_type = wr_ws.normal_type,
		.invisible = wr_ws.invisible,
		.collisions = wr_ws.collisions,
		.uv_offset = wr_ws.uv_offset,
		.uv_repeat = wr_ws.uv_repeat
	};
	if (!append_wall_sections_array(&*sections, ws))
		return (FALSE);
	return (TRUE);
}

t_bool		read_wall(t_ressource_manager *r, t_walls **walls)
{
	t_wr_wall		wr_wall;
	t_wall			wall;
	int				i;

	if (!io_memcpy(&r->reader, &wr_wall, sizeof(t_wr_wall)))
		return (FALSE);
	wall = (t_wall) {
		.indice = wr_wall.indice,
		.floor_height = wr_wall.floor_height,
		.ceiling_height = wr_wall.ceiling_height
	};
	if (wr_wall.wall_sections_count < 0)
		return (FALSE);
	if (!(wall.wall_sections = create_wall_sections_array(
		wr_wall.wall_sections_count + 5)))
		return (FALSE);
	i = -1;
	while (++i < wr_wall.wall_sections_count)
		if (!read_wall_section(r, &wall.wall_sections))
			return (FALSE);
	if (!append_walls_array(&*walls, wall))
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

t_bool		read_sprite(t_ressource_manager *r, t_sprite **sprite)
{
	t_wr_sprite		wr_sprite;
	t_sprite		*s;

	if (!io_memcpy(&r->reader, &wr_sprite, sizeof(t_wr_sprite)))
		return (FALSE);
	if (wr_sprite.texture_index < 0
		|| wr_sprite.texture_index >= r->ressources->len)
		return (FALSE);
	if (!(s = create_sprite((t_vec2){ 1, 1 },
		r->ressources->values[wr_sprite.texture_index])))
		return (FALSE);
	s->always_facing_player = wr_sprite.always_facing_player;
	s->hitbox_radius = wr_sprite.hitbox_radius;
	*sprite = s;
	return (TRUE);
}

t_bool		read_entity(t_ressource_manager *r, t_entity_type *type)
{
	if (!io_memcpy(&r->reader, type, sizeof(t_entity_type)))
		return (FALSE);
	return (TRUE);
}
