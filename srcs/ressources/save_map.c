/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:20:07 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/02 16:12:54 by lloncham         ###   ########.fr       */
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
	t_wr_sprite	wr_sprite;

	wr_sprite = (t_wr_sprite) {
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

t_bool		write_object_light(t_ressource_manager *r, int light_index)
{
	const t_light	light = r->doom->lights->values[light_index];

	dp_write(r, &light, sizeof(t_light));
	return (TRUE);
}

t_bool		write_object_transpo(t_ressource_manager *r, t_transpo *transpo)
{
	dp_write(r, transpo, sizeof(t_transpo));
	return (TRUE);
}

t_bool		write_object(t_ressource_manager *r, t_object *object)
{
	t_wr_object wr_object;

	wr_object = (t_wr_object) {
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
	else if (object->type == OBJECT_LIGHT)
		write_object_light(r, object->of.light_index);
	else if (object->type == OBJECT_TRANSPO)
		write_object_transpo(r, object->of.transpo);
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
