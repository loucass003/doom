/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_point.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 20:10:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/10 21:56:43 by llelievr         ###   ########.fr       */
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

	pos = editor->points->vertices[editor->current_point];
	editor->points->vertices[editor->current_point] = editor->grid_cell;
	cancel = FALSE;
	i = -1;
	while (++i < editor->rooms->len)
	{
		t_room *room = &editor->rooms->values[i];
		int index = wall_indexof_by_indice(room->walls, editor->current_point);
		if (index != -1)
		{
			if (room_intersect(editor, room, room, TRUE))
			{
				cancel = TRUE;
				break;
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
	}
}