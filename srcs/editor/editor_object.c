/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 18:26:47 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 11:44:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		draw_circle(&editor->doom->screen, (t_pixel){ object->pos.x, object->pos.z, color }, 8);
	}
	return (TRUE);
}

t_object	init_object(t_editor *editor, t_vec2 pos)
{
	t_object		object;
	float			y;

	int room = point_in_rooms(editor, pos);
	y = 0;
	if (room != -1)
		y = room_height_range(&editor->rooms->values[room]).x;
	object = (t_object){ .pos = (t_vec3){ pos.x, y, pos.y } };
	set_object_default(editor->doom, &object);
	return object;
}

void		free_object(t_object *o)
{
	if (o->type == OBJECT_SPRITE)
		ft_memdel((void **)&o->of.sprite);
	else if (o->type == OBJECT_ITEMSTACK)
		free_itemstack(&o->of.itemstack);
}

void		free_objects(t_objects **objects)
{
	int			i;
	t_object	*o;

	if (!*objects)
		return;
	i = -1;
	while (++i < (*objects)->len)
	{
		o = &(*objects)->values[i];
		free_object(o);
	}
	ft_memdel((void **)objects);
}