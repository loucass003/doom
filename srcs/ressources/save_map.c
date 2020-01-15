/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:20:07 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 20:16:37 by llelievr         ###   ########.fr       */
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
	const t_wr_wall_section	wr_ws = (t_wr_wall_section) {
		.resource_index = ressources_indexof(r->ressources, ws->texture),
		.normal_type = ws->normal_type,
		.invisible = ws->invisible,
		.collisions = ws->collisions
	};

	dp_write(r, &wr_ws, sizeof(t_wr_wall_section));
	return (TRUE);
}

t_bool		write_wall(t_ressource_manager *r, t_wall *wall)
{
	const t_wr_wall	wr_wall = (t_wr_wall) {
		.indice = wall->indice,
		.floor_height = wall->floor_height,
		.ceiling_height = wall->ceiling_height,
		.wall_sections_count = wall->wall_sections ? wall->wall_sections->len : 0
	};
	int				i;

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
	const t_wr_room	wr_room = (t_wr_room) {
		.closed = room->closed && room->walls->len >= 3,
		.walls_count = room->walls->len,
		.floor_res_index = ressources_indexof(r->ressources, room->floor_texture),
		.ceiling_res_index = ressources_indexof(r->ressources, room->ceiling_texture),
		.floor_rot = room->floor_rot,
		.ceil_rot = room->ceil_rot,
		.floor_visible = room->floor_visible,
		.ceil_visible = room->ceil_visible,
		.ambiant_light = room->ambiant_light
	};

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

t_bool		write_points(t_ressource_manager *r)
{
	dp_write(r, &r->doom->editor.points->len, sizeof(int));
	dp_write(r, &r->doom->editor.points->vertices,
		sizeof(t_vec2) * r->doom->editor.points->len);
	return (TRUE);
}

t_bool		write_item(t_ressource_manager *r, t_item *item)
{
	dp_write(r, &item->type, sizeof(t_item_type));
	if (item->type == ITEM_WEAPON)
		dp_write(r, &item->data.weapon.type, sizeof(t_weapon_type));
	return (TRUE);
}

t_bool		write_itemstack(t_ressource_manager *r, t_itemstack *is)
{
	dp_write(r, &is->amount, sizeof(int));
	write_item(r, is->of);
	return (TRUE);
}

t_bool		write_sprite(t_ressource_manager *r, t_sprite *sprite)
{
	const	t_wr_sprite	wr_sprite = (t_wr_sprite) {
		.always_facing_player = sprite->always_facing_player,
		.texture_index = ressources_indexof(r->ressources, sprite->texture),
		.hitbox_radius = sprite->hitbox_radius
	};

	dp_write(r, &wr_sprite, sizeof(t_wr_sprite));
	return (TRUE);
}

t_bool		write_entity(t_ressource_manager *r, t_entity_type entity_type)
{
	dp_write(r, &entity_type, sizeof(t_entity_type));
	return (TRUE);
}

t_bool		write_object_model(t_ressource_manager *r, t_ressource *model)
{
	const int index = ressources_indexof(r->ressources, model);

	dp_write(r, &index, sizeof(int));
	return (TRUE);
}

t_bool		write_object(t_ressource_manager *r, t_object *object)
{
	const t_wr_object	wr_object = (t_wr_object) {
		.type = object->type,
		.pos = object->pos,
		.scale = object->scale,
		.rotation = object->rotation,
		.no_light = object->no_light
	};

	dp_write(r, &wr_object, sizeof(t_wr_object));
	if (object->type == OBJECT_ITEMSTACK)
		write_itemstack(r, object->of.itemstack);
	else if (object->type == OBJECT_SPRITE)
		write_sprite(r, object->of.sprite);
	else if (object->type == OBJECT_ENTITY)
		write_entity(r, object->of.entity);
	else if (object->type == OBJECT_MODEL)
		write_object_model(r, object->of.model);
	return (TRUE);
}

t_bool		write_objects(t_ressource_manager *r)
{
	int		i;

	dp_write(r, &r->doom->editor.objects->len, sizeof(int));
	i = -1;
	while (++i < r->doom->editor.objects->len)
		if (!write_object(r, &r->doom->editor.objects->values[i]))
			return (FALSE);
	return (TRUE);
}

t_bool		write_player(t_ressource_manager *r)
{
	const t_wr_player	wr_player = (t_wr_player){
		.set = r->doom->editor.player_set,
		.position = r->doom->player.spawn_data.position,
		.rotation = r->doom->player.spawn_data.rotation
	};

	dp_write(r, &wr_player, sizeof(t_wr_player));
	return (TRUE);
}

t_bool		write_globals(t_ressource_manager *r)
{
	const t_wr_globals	wr_globals = (t_wr_globals){
		.skybox = r->doom->skybox_enabled
	};

	dp_write(r, &wr_globals, sizeof(t_wr_globals));
	return (TRUE);
}	

t_bool		write_map(t_ressource_manager *r)
{
	write_globals(r);
	write_points(r);
	write_rooms(r);
	write_objects(r);
	write_player(r);
	return (TRUE);
}
