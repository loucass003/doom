/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_object.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 17:24:14 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 20:24:09 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "editor.h"

void			editor_tool_objects_move(t_editor *editor)
{
	t_object	*object;

	if (editor->current_object != -1 && editor->object_grab
		&& editor->grid_cell.x != -1)
	{
		object = &editor->objects->values[editor->current_object];
		object->pos.x = editor->grid_cell.x;
		object->pos.z = editor->grid_cell.y;
	}
}

void			editor_tool_objects_release(t_editor *editor)
{
	if (editor->current_object != -1 && editor->object_grab)
		editor->object_grab = FALSE;
}

void			editor_tool_objects(t_editor *editor)
{
	if (editor->current_object == -1 && editor->grid_cell_grab != GG_PLAYER
		&& editor->grid_cell_grab != GG_OBJECT
		&& editor->grid_cell_grab != GG_OUTSIDE)
	{
		if (!append_objects_array(&editor->objects,
			init_object(editor, editor->grid_cell)))
			return ;
		editor->current_object = -1;
		editor->grid_cell_grab = GG_OBJECT;
		editor->close_object = editor->objects->len - 1;
	}
	else if (editor->close_object != -1 && editor->grid_cell_grab == GG_OBJECT)
	{
		editor->current_object = editor->close_object;
		editor->object_grab = TRUE;
		editor_settings_update(editor);
	}
	if (editor->current_object != -1 && (editor->grid_cell_grab == GG_NONE))
	{
		editor->current_object = -1;
		editor_settings_update(editor);
	}
}
