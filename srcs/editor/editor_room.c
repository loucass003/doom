/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_room.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:37:59 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/10 01:07:24 by llelievr         ###   ########.fr       */
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
		if (!contain_point && !!(i = -1))
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


void			select_floor_ceil(t_editor *editor, t_room *room, t_bool floor)
{
	t_room		*prev_room;

	if (editor->current_room != -1 && editor->selected_floor_ceil != -1)
	{
		prev_room = &editor->rooms->values[editor->current_room];
		if (prev_room->r)
			prev_room->r->materials->values[editor->selected_floor_ceil].texture_map_set = TRUE;
		editor->selected_floor_ceil = -1;
	}

	if (!room || !room->r)
		return ;
	editor->selected_floor_ceil = floor ? 0 : 1;
	room->r->materials->values[editor->selected_floor_ceil].texture_map_set = FALSE;
}

void			select_room(t_editor *editor, int index)
{
	editor->current_object = -1;
	editor->selected_floor_ceil = -1;
	editor->current_room = index;
	editor_settings_update(editor);
}


t_bool			point_in_room(t_editor *editor, t_room *room, t_vec2 point)
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

t_vec2			room_height_range(t_editor *editor, t_room *room)
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

t_vec3			room_center(t_editor *editor, t_room *room)
{
	int			i;
	t_vec3		min;
	t_vec3		max;
	t_wall		wall;
	t_vec2		point;

	min = (t_vec3){ INT_MAX, 0, INT_MAX };
	max = (t_vec3){ INT_MIN, 0, INT_MIN };
	i = -1;
	while (++i < room->walls->len)
	{
		wall = room->walls->values[i];
		point = editor->points->vertices[wall.indice];
		min.x = fmin(point.x, min.x);
		// min.y = fmin(wall.floor_height, min.y);
		// min.y = fmin(wall.ceiling_height, min.y);
		min.z = fmin(point.y, min.z);
		max.x = fmax(point.x, max.x);
		// max.y = fmax(wall.floor_height, max.y);
		// max.y = fmax(wall.ceiling_height, max.y);
		max.z = fmax(point.y, max.z);
	}

	t_vec3 pos = ft_vec3_add(min, ft_vec3_div_s(ft_vec3_sub(max, min), 2));
	printf("%f %f %f\n", pos.x, pos.y, pos.z);
	return (ft_vec3_add(min, ft_vec3_div_s(ft_vec3_sub(max, min), 2)));
}

int				point_in_rooms(t_editor *editor, t_vec2 point)
{
	int			i;
	t_room		*room;
	int			index;

	i = -1;
	index = -1;
	while (++i < editor->rooms->len)
	{
		room = &editor->rooms->values[i];
		if (point_in_room(editor, room, point))
			index = i;
	}
	return (index);
}


void			get_room_gaps(t_editor *editor, t_room *room)
{
	int			i;
	int			j;
	t_wall		*wall0;
	t_wall		*wall1;

	printf("CALL\n");
	i = -1;
	while (++i < room->walls->len)
	{
		j = -1;
		wall0 = &room->walls->values[i];
		wall1 = &room->walls->values[(i + 1) % room->walls->len];
		while (++j < editor->rooms->len)
		{
			if (room == &editor->rooms->values[j])
				continue;
			t_room *curr_room = &editor->rooms->values[j];
			int index0 = wall_indexof_by_indice(curr_room->walls, wall0->indice);
			int index1 = wall_indexof_by_indice(curr_room->walls, wall1->indice);
			// int w_index = wall_indexof_by_indice(editor->rooms->values[j].walls, wall0.indice);
			printf("-- %d %d\n", index0, index1);
			if (index0 == -1 || index1 == -1)
				continue;
			t_wall wall2 = editor->rooms->values[j].walls->values[index0];
			t_wall wall3 = editor->rooms->values[j].walls->values[index1];
			printf("ROOM %d\n", j);
			printf("WALL0 %f %f\n", wall2.floor_height, wall2.ceiling_height);
			printf("WALL1 %f %f\n", wall3.floor_height, wall3.ceiling_height);
			if (wall2.floor_height != wall0->floor_height || wall2.ceiling_height != wall0->ceiling_height)
			{
				if (!wall0->start_rooms_range && !(wall0->start_rooms_range = create_2dvertices_array(5)))
					return ;
				append_2dvertices_array(&wall0->start_rooms_range, (t_vec2){ wall2.floor_height, wall2.ceiling_height });
			//	printf("WALL0 %f %f\n", wall2.floor_height, wall2.ceiling_height);
			}
			if (wall3.floor_height != wall1->floor_height && wall3.ceiling_height != wall1->ceiling_height)
			{
				if (!wall1->end_rooms_range && !(wall1->end_rooms_range = create_2dvertices_array(5)))
					return ;
				append_2dvertices_array(&wall1->end_rooms_range, (t_vec2){ wall3.floor_height, wall3.ceiling_height });
				// printf("WALL1 %f %f\n", wall3.floor_height, wall3.ceiling_height);
			}
		}
		if (wall0->start_rooms_range)
			sort_ranges(wall0->start_rooms_range);
		if (wall0->end_rooms_range)
			sort_ranges(wall0->end_rooms_range);
	}
	
}
