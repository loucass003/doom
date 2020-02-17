/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_point.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 20:10:29 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/17 15:04:15 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void		editor_tool_point_release(t_editor *editor)
{
	editor->current_point = -1;
}

void		editor_tool_point_move(t_editor *editor)
{
	int		i;
	t_bool	cancel;
	t_vec2	pos;
	int		index;
	t_room	*room;

	pos = editor->points->vertices[editor->current_point];
	editor->points->vertices[editor->current_point] = editor->grid_cell;
	cancel = FALSE;
	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		index = wall_indexof_by_indice(room->walls, editor->current_point);
		if (index != -1)
		{
			if (room_intersect(editor, room, room, TRUE))
			{
				cancel = TRUE;
				break ;
			}
		}
	}
	if (cancel)
		editor->points->vertices[editor->current_point] = pos;
}

void		editor_tool_point(t_editor *editor)
{
	int index;

	if (editor->grid_cell_grab == GG_LINE)
	{
		append_2dvertices_array(&editor->points, editor->grid_cell);
		insert_point(editor, editor->close_seg,
			editor->points->len - 1);
	}
	else if (editor->grid_cell_grab == GG_POINT)
	{
		index = vertices2d_indexof(editor->points, editor->grid_cell);
		if (index != -1)
			editor->current_point = index;
		update_rooms_gaps(editor);
	}
}
