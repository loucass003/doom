/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tool_gg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 15:37:27 by louali            #+#    #+#             */
/*   Updated: 2020/02/17 15:37:49 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include "ressource.h"

void		gg_point(t_editor *editor, t_room *curr_room, int index)
{
	append_walls_array(&curr_room->walls, init_wall(index));
	if (room_intersect(editor, curr_room, curr_room, TRUE))
		splice_walls_array(curr_room->walls, curr_room->walls->len - 1, 1);
	else
		editor->line_start_cell = editor->grid_cell;
	update_rooms_gaps(editor);
}

t_bool		gg_line(t_editor *editor, t_room *curr_room, int index)
{
	append_walls_array(&curr_room->walls, init_wall(index));
	if (room_intersect(editor, curr_room, curr_room, TRUE))
	{
		splice_2dvertices_array(editor->points, index, 1);
		splice_walls_array(curr_room->walls,
		curr_room->walls->len - 1, 1);
		return (TRUE);
	}
	else
		editor->line_start_cell = editor->grid_cell;
	insert_point(editor, editor->close_seg, index);
	return (FALSE);
}

void		gg_none(t_editor *editor, t_room *curr_room, int index)
{
	append_walls_array(&curr_room->walls, init_wall(index));
	if (room_intersect(editor, curr_room, curr_room, TRUE))
	{
		splice_2dvertices_array(editor->points, index, 1);
		splice_walls_array(curr_room->walls,
			curr_room->walls->len - 1, 1);
	}
	else
		editor->line_start_cell = editor->grid_cell;
}
