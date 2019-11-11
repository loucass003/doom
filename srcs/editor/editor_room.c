/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_room.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:37:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/10 19:34:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

int			get_close_room(t_editor *editor)
{
	int		i;
	int		index;
	t_room	*room;

	i = -1;
	while (++i < editor->rooms->len)
	{
		if (editor->grid_cell.x == -1 || editor->grid_cell_grab == GG_NONE)
			continue;
		room = &editor->rooms->values[i];
		if (editor->grid_cell_grab == GG_POINT)
		{
			index = vertices2d_indexof(editor->points, editor->grid_cell);
			if (wall_indexof_by_indice(room->walls, index) != -1)
				return (i);
		}
		else if (editor->grid_cell_grab == GG_LINE)
		{
			if (wall_indexof_by_indice(room->walls, editor->close_seg.x) != -1
				&& wall_indexof_by_indice(room->walls,
					editor->close_seg.y) != -1)
				return (i);
		}
	}
	return (-1);
}

void			remove_room(t_editor *editor, int index)
{
	t_room		*room;
	int			i;
	int			j;
	int			indice;
	t_bool		contain_point;

	room = &editor->rooms->values[index];
	i = -1;
	while (++i < room->walls->len && !(j = -1))
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
		if (!contain_point && !(i = -1))
			remove_point(editor, indice);
	}
	splice_rooms_array(editor->rooms, index, 1);
}

void			editor_render_rooms(t_gui *self, t_doom *doom, t_editor *editor)
{
	int		i;
	t_room	*room;

	i = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (room->walls->len < 2)
			continue;
		int j = -1;
		if (!room->closed)
		{
			t_vec2	first_point = editor->points->vertices[room->walls->values[0].indice];
			draw_circle(&doom->screen, (t_pixel){ first_point.x, first_point.y, 0xFFFFF00F }, 6);
		}
		while (++j < room->walls->len - !(room->closed))
			if (!editor_render_wall(doom, editor, room, j))
				continue;
	}
}