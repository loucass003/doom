/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_room_mod.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:36:46 by louali            #+#    #+#             */
/*   Updated: 2020/04/12 18:06:13 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include <limits.h>

void		update_array_room(t_editor *editor, int index)
{
	splice_rooms_array(editor->rooms, index, 1);
	update_rooms_gaps(editor);
}

void		remove_room(t_editor *editor, int index)
{
	t_room		*room;
	int			i;
	int			j;
	int			indice;
	t_bool		contain_point;

	room = &editor->rooms->values[index];
	i = -1;
	while (++i < room->walls->len && !!(j = -1))
	{
		indice = room->walls->values[i].indice;
		contain_point = FALSE;
		while (++j < editor->rooms->len)
		{
			if (room == &editor->rooms->values[j])
				continue;
			if (wall_indexof_by_indice(editor->rooms->values[j].walls,
				indice) != -1)
				contain_point = TRUE;
		}
		splice_walls_array(room->walls, i--, 1);
		(!contain_point && !!(i = -1)) ? remove_point(editor, indice) : 0;
	}
	update_array_room(editor, index);
}

void		select_room(t_editor *editor, int index)
{
	editor->current_object = -1;
	editor->selected_floor_ceil = -1;
	editor->current_room = index;
	editor->slope_mode = FALSE;
	editor_settings_update(editor);
}

t_vec2		room_height_range(t_room *room)
{
	int			i;
	t_vec2		range;
	t_wall		wall;

	range = (t_vec2){ INT_MIN, INT_MAX };
	i = -1;
	while (++i < room->walls->len)
	{
		wall = room->walls->values[i];
		range.x = fmax(wall.floor_height, range.x);
		range.y = fmin(wall.ceiling_height, range.y);
	}
	return (range);
}
