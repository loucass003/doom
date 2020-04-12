/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 22:38:06 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/12 17:44:32 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

void		remove_all_room(t_editor *editor, int i)
{
	select_room(editor, -1);
	remove_room(editor, i);
}

static void	editor_remove_point(t_editor *editor)
{
	int		i;
	int		index;
	int		index0;
	t_bool	rem_room;

	if ((index = vertices2d_indexof(editor->points, editor->grid_cell)) == -1)
		return ;
	rem_room = FALSE;
	i = -1;
	while (++i < editor->rooms->len)
	{
		if ((index0 = wall_indexof_by_indice(editor->rooms->values[i].walls,
			index)) == -1)
			continue;
		if (editor->rooms->values[i].walls->len - 1 < 3)
		{
			remove_all_room(editor, i);
			rem_room = TRUE;
			break ;
		}
		else
			splice_walls_array(editor->rooms->values[i].walls, index0, 1);
	}
	!rem_room ? remove_point(editor, index) : 0;
}

void					remove_light(t_doom *doom, int light_index)
{
	int			i;
	t_object	*object;

	i = -1;
	splice_lights_array(doom->lights, light_index, 1);
	while (++i < doom->editor.objects->len)
	{
		object = &doom->editor.objects->values[i];
		if (object->type == OBJECT_LIGHT && object->of.light_index > light_index)
			object->of.light_index--;
	}
}

void		editor_delete_action(t_editor *editor)
{
	t_object	*object;

	if (editor->grid_cell_grab == GG_POINT)
		editor_remove_point(editor);
	else if (editor->current_room != -1)
	{
		remove_room(editor, editor->current_room);
		select_room(editor, -1);
		editor->line_start_cell = (t_vec2){ -1, -1 };
	}
	else if (editor->current_object != -1)
	{
		object = &editor->objects->values[editor->current_object];
		if (object->type == OBJECT_LIGHT)
			remove_light(editor->doom, object->of.light_index);
		splice_objects_array(editor->objects, editor->current_object, 1);
		editor->current_object = -1;
		editor_settings_update(editor);
	}
}
