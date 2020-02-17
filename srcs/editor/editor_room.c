/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_room.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:37:59 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/14 14:38:31 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include <limits.h>

int			get_close_room(t_editor *editor)
{
	int		i;
	int		index;
	t_room	*room;
	int		index2;
	int		index1;

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
			index1 = wall_indexof_by_indice(room->walls, editor->close_seg.x);
			index2 = wall_indexof_by_indice(room->walls, editor->close_seg.y);
			if (index1 != -1 && index2 != -1 && ((index2 == index1 + 1 || index2
			== index1 - 1) || ((index1 == 0 && index2 == room->walls->len - 1)
			|| (index2 == 0 && index1 == room->walls->len - 1))))
				return (i);
		}
	}
	return (-1);
}

void		select_floor_ceil(t_editor *editor, int room_index,
	t_bool floor)
{
	int		material_index;
	t_room	*room;

	if (editor->current_room != -1 && editor->selected_floor_ceil != -1)
	{
		room = &editor->rooms->values[editor->current_room];
		material_index = editor->current_room * 2 + editor->selected_floor_ceil;
		get_map(editor)->materials
		->values[material_index].texture_map_set = TRUE;
		editor->selected_floor_ceil = -1;
	}
	if (room_index == -1)
		return ;
	room = &editor->rooms->values[room_index];
	material_index = room_index * 2 + !floor;
	editor->selected_floor_ceil = floor ? 0 : 1;
	if (editor->slope_mode)
		get_map(editor)->materials
		->values[material_index].texture_map_set = FALSE;
}

t_bool		point_in_room(t_editor *editor, t_room *room, t_vec2 point)
{
	t_bool		inside;
	int			i;
	int			j;
	t_line		seg;

	inside = FALSE;
	i = -1;
	while (++i < room->walls->len)
	{
		j = (i + 1) % room->walls->len;
		seg.a = editor->points->vertices[room->walls->values[i].indice];
		seg.b = editor->points->vertices[room->walls->values[j].indice];
		if ((seg.a.y <= point.y && seg.b.y > point.y)
			|| (seg.b.y <= point.y && seg.a.y > point.y))
		{
			if (((seg.b.x - seg.a.x) * (point.y - seg.a.y)
				/ (seg.b.y - seg.a.y) + seg.a.x) < point.x)
				inside = !inside;
		}
	}
	return (inside);
}

t_vec3		room_center(t_editor *editor, t_room *room)
{
	int			i;
	t_vec3		min;
	t_vec3		max;
	t_vec2		point;
	t_vec3		pos;

	min = (t_vec3){ INT_MAX, 0, INT_MAX };
	max = (t_vec3){ INT_MIN, 0, INT_MIN };
	i = -1;
	while (++i < room->walls->len)
	{
		point = editor->points->vertices[room->walls->values[i].indice];
		min.x = fmin(point.x, min.x);
		min.z = fmin(point.y, min.z);
		max.x = fmax(point.x, max.x);
		max.z = fmax(point.y, max.z);
	}
	pos = ft_vec3_add(min, ft_vec3_div_s(ft_vec3_sub(max, min), 2));
	printf("%f %f %f\n", pos.x, pos.y, pos.z);
	return (ft_vec3_add(min, ft_vec3_div_s(ft_vec3_sub(max, min), 2)));
}
