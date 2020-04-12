/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 18:26:47 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/12 17:58:44 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include "editor.h"
#include "doom.h"

t_bool		get_close_object(t_editor *editor, t_vec2 *pos)
{
	int			i;
	t_object	*object;

	editor->close_object = -1;
	i = -1;
	while (++i < editor->objects->len)
	{
		object = &editor->objects->values[i];
		if (is_in_range((t_vec2){ object->pos.x, object->pos.z }, *pos))
		{
			editor->grid_cell_grab = GG_OBJECT;
			editor->close_object = i;
			*pos = (t_vec2){ object->pos.x, object->pos.z };
			return (TRUE);
		}
	}
	return (FALSE);
}

int			get_object(t_editor *editor, t_vec2 pos)
{
	int			i;
	t_object	object;

	i = -1;
	while (++i < editor->objects->len)
	{
		object = editor->objects->values[i];
		if (object.pos.x == pos.x && object.pos.z == pos.y)
			return (i);
	}
	return (-1);
}

t_bool		editor_render_objects(t_editor *editor)
{
	int			i;
	int			color;
	t_object	*object;

	i = -1;
	while (++i < editor->objects->len)
	{
		object = &editor->objects->values[i];
		color = i == editor->current_object ? 0xFFAAFFAA : 0xFF00AA00;
		draw_circle(&editor->doom->screen, (t_pixel){ object->pos.x,
			object->pos.z, color }, 8);
	}
	return (TRUE);
}

t_object	init_object(t_editor *editor, t_vec2 pos)
{
	t_object	object;
	float		y;
	int			room;

	room = point_in_rooms(editor, pos);
	y = 0;
	if (room != -1)
		y = room_height_range(&editor->rooms->values[room]).x;
	object = (t_object){ .pos = (t_vec3){ pos.x, y, pos.y } };
	set_object_default(editor->doom, &object);
	return (object);
}

void			set_object_default(t_doom *doom, t_object *object)
{
	object->no_light = FALSE;
	if (object->type == OBJECT_ITEMSTACK)
		object->of.itemstack = create_itemstack_from_type(doom, ITEM_AMMO, -1);
	else if (object->type == OBJECT_ENTITY)
		object->of.entity = ENTITY_ENEMY;
	else if (object->type == OBJECT_SPRITE)
		object->of.sprite = create_sprite((t_vec2){ 1, 1 },
			get_default_texture(&doom->res_manager, TRUE));
	else if (object->type == OBJECT_MODEL)
		object->of.model = get_ressource(&doom->res_manager, RESSOURCE_MODEL);
	else if (object->type == OBJECT_TRANSPO)
		object->of.transpo = create_default_transpo(doom, object);
	else if (object->type == OBJECT_LIGHT)
	{
		object->of.light_index = create_default_light(doom);
		object->no_light = TRUE;
	}
	object->scale = (t_vec3){0, 0, 0};
	object->rotation = (t_vec3){0, 0, 0};
	if (object->r)
		create_object_renderable(&doom->editor,
			objects_indexof(doom->editor.objects, object), object->r);
}
