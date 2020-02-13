/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:46:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/13 16:01:33 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			remove_point(t_editor *editor, int index)
{
	int		i;
	t_walls	*walls;
	int		*v;
	int		j;

	splice_2dvertices_array(editor->points, index, 1);
	i = -1;
	while (++i < editor->rooms->len)
	{
		j = -1;
		walls = editor->rooms->values[i].walls;
		while (++j < walls->len)
		{
			v = &walls->values[j].indice;
			if (*v > index)
				*v = *v - 1;
		}
	}
}

void			point_room(int index1, t_room *room0,
	int point_index, int index0)
{
	float	floor_h;
	float	ceil_h;
	int		index;

	append_walls_array(&room0->walls, init_wall(-20));
	floor_h = room0->walls->values[index1].floor_height;
	ceil_h = room0->walls->values[index1].ceiling_height;
	index = index1;
	if (index1 == room0->walls->len - 2 && index0 == 0)
		index = room0->walls->len - 1;
	else
		ft_memmove(room0->walls->values + index1 + 1,
			room0->walls->values + index1, (room0->walls->len
			- (index1)) * sizeof(t_wall));
	room0->walls->values[index] = init_wall(point_index);
	room0->walls->values[index].floor_height = floor_h;
	room0->walls->values[index].ceiling_height = ceil_h;
}

void			insert_point(t_editor *editor, t_vec2 seg, int point_index)
{
	int		i;
	int		tmp;
	int		index1;
	int		index0;
	t_room	*room0;

	i = -1;
	while (++i < editor->rooms->len)
	{
		room0 = &editor->rooms->values[i];
		if (!room0->closed)
			continue;
		index0 = wall_indexof_by_indice(room0->walls, seg.x);
		index1 = wall_indexof_by_indice(room0->walls, seg.y);
		if (index0 > index1)
		{
			tmp = index0;
			index0 = index1;
			index1 = tmp;
		}
		if (index0 != -1 && index1 != -1)
			point_room(index1, room0, point_index, index0);
	}
	update_rooms_gaps(editor);
}

t_vec2			get_point_on_seg(t_vec2 p0, t_vec2 p1, t_vec2 pos)
{
	const	t_vec2	b = ft_vec2_sub(p1, p0);
	const	t_vec2	p = ft_vec2_sub(pos, p0);
	float			f;
	float			l;

	l = ft_vec2_len(b);
	f = clamp(0, 1, ft_vec2_dot(p, b) / l / l);
	return ((t_vec2){ p0.x + f * b.x, p0.y + f * b.y });
}

t_bool			is_point_on_seg(t_vec2 project, t_vec2 pos)
{
	return (ft_vec2_len(ft_vec2_sub(project, pos)) < 6);
}

t_bool			is_in_range(t_vec2 pos, t_vec2 test)
{
	return ((test.x - pos.x) * (test.x - pos.x) + (test.y - pos.y)
		* (test.y - pos.y) < 9 * 9);
}

t_vec2			get_close_seg(t_editor *editor, t_room *room, t_vec2 pos)
{
	int		i;
	t_vec2	p0;
	t_vec2	p1;
	t_vec2	project;

	i = -1;
	while (++i < room->walls->len - !(room->closed))
	{
		p0 = editor->points->vertices[room->walls->values[i].indice];
		p1 = editor->points->vertices[room->walls->values[(i + 1)
			% room->walls->len].indice];
		project = get_point_on_seg(p0, p1, pos);
		if (is_point_on_seg(project, pos))
			return ((t_vec2){ room->walls->values[i].indice,
				room->walls->values[(i + 1) % room->walls->len].indice });
	}
	return ((t_vec2){ -1, -1 });
}

t_vec2			point_on_room(t_editor *editor, t_room *room,
	t_vec2 pos, t_bool *found)
{
	int			k;
	t_vec2		project;
	t_vec2		p1;
	t_vec2		p0;

	*found = TRUE;
	k = -1;
	while (++k < room->walls->len - !(room->closed))
	{
		p0 = editor->points->vertices[room->walls->values[k].indice];
		p1 = editor->points->vertices[room->walls->values[(k + 1)
			% room->walls->len].indice];
		editor->grid_cell_grab = GG_POINT;
		if (is_in_range(p0, pos))
			return (p0);
		if (is_in_range(p1, pos))
			return (p1);
		project = get_point_on_seg(p0, p1, pos);
		if (is_point_on_seg(project, pos))
		{
			editor->grid_cell_grab = GG_LINE;
			editor->close_seg = (t_vec2){ room->walls->values[k].indice,
				room->walls->values[(k + 1) % room->walls->len].indice };
			return (project);
		}
	}
	*found = FALSE;
	return (pos);
}

t_vec2			get_close_point(t_editor *editor, t_vec2 pos)
{
	int		j;
	t_vec2	p;
	t_bool	found;
	t_room	*room2;

	p = pos;
	if ((editor->selected_tool == TOOL_OBJECTS || editor->selected_tool
		== TOOL_SELECT) && get_close_object(editor, &p))
		return (p);
	j = -1;
	while (++j < editor->rooms->len)
	{
		room2 = &editor->rooms->values[j];
		found = FALSE;
		p = point_on_room(editor, room2, pos, &found);
		if (found)
			return (p);
	}
	if (editor->player_set
		&& is_in_range((t_vec2){editor->doom->player.spawn_data.position.x,
		editor->doom->player.spawn_data.position.z}, p))
	{
		editor->grid_cell_grab = GG_PLAYER;
		return ((t_vec2){editor->doom->player.spawn_data.position.x,
		editor->doom->player.spawn_data.position.z});
	}
	editor->grid_cell_grab = GG_NONE;
	if (!in_bounds((SDL_Rect){ 10, 70, S_WIDTH - 20, S_HEIGHT - 80 }, pos)
		|| (is_settings_open(editor) && in_bounds((SDL_Rect){ S_WIDTH - 335,
		75, 320, 550 }, pos)))
		editor->grid_cell_grab = GG_OUTSIDE;
	return (pos);
}
