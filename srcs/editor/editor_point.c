/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:46:30 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/17 23:10:33 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			remove_point(t_editor *editor, int index)
{
	int		i;
	t_walls	*walls;

	splice_2dvertices_array(editor->points, index, 1);

	i = -1;
	while (++i < editor->rooms->len)
	{
		int j = -1;
		walls = editor->rooms->values[i].walls;
		while (++j < walls->len)
		{
			int *v = &walls->values[j].indice;
			if (*v > index)
				*v = *v - 1;
		}
	}
}

void			insert_point(t_editor *editor, t_vec2 seg, int point_index)
{
	int		i;

	i = -1;
	while (++i < editor->rooms->len)
	{
		t_room	*room0 = &editor->rooms->values[i];
		int		index0 = wall_indexof_by_indice(room0->walls, seg.x);
		int		index1 = wall_indexof_by_indice(room0->walls, seg.y);
		
		if (index0 > index1 && index1 != 0)
		{
			int tmp = index0;
			index0 = index1;
			index1 = tmp;
		}

		if (index0 != -1 && index1 != -1)
		{
			append_walls_array(&room0->walls, init_wall(editor, -20));
			ft_memmove(room0->walls->values + index1 + 1, room0->walls->values + index1, (room0->walls->len - (index1 + 1)) * sizeof(t_wall));
			room0->walls->values[index1] = init_wall(editor, point_index);
		}
	}
}

t_vec2			get_point_on_seg(t_vec2 p0, t_vec2 p1, t_vec2 pos)
{
	const	t_vec2	b = ft_vec2_sub(p1, p0);
	const	t_vec2	p = ft_vec2_sub(pos, p0);
	float	f;
	float	l;
	
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
	return ((test.x - pos.x) * (test.x - pos.x) + (test.y - pos.y) * (test.y - pos.y) < 9 * 9);
}

t_vec2			get_close_seg(t_editor *editor, t_room *room, t_vec2 pos)
{
	int		i;

	i = -1;
	while (++i < room->walls->len - !(room->closed))
	{
		t_vec2 p0 = editor->points->vertices[room->walls->values[i].indice];
		t_vec2 p1 = editor->points->vertices[room->walls->values[(i + 1) % room->walls->len].indice];
		t_vec2	project = get_point_on_seg(p0, p1, pos);
		if (is_point_on_seg(project, pos))
			return (t_vec2){ room->walls->values[i].indice, room->walls->values[(i + 1) % room->walls->len].indice };
	}
	return (t_vec2){ -1, -1 };
}

t_vec2			point_on_room(t_editor *editor, t_room *room, t_vec2 pos, t_bool *found)
{
	int			k;

	*found = TRUE;
	k = -1;
	while (++k < room->walls->len - !(room->closed))
	{
		t_vec2 p0 = editor->points->vertices[room->walls->values[k].indice];
		t_vec2 p1 = editor->points->vertices[room->walls->values[(k + 1) % room->walls->len].indice];
		editor->grid_cell_grab = GG_POINT;
		if (is_in_range(p0, pos))
			return p0;
		if (is_in_range(p1, pos))
			return p1;
		t_vec2	project = get_point_on_seg(p0, p1, pos);
		if (is_point_on_seg(project, pos))
		{
			editor->grid_cell_grab = GG_LINE;
			editor->close_seg = (t_vec2){ room->walls->values[k].indice, room->walls->values[(k + 1) % room->walls->len].indice };
			return project;
		}
	}
	*found = FALSE;
	return (pos);
}

t_vec2			get_close_point(t_editor *editor, t_vec2 pos)
{
	int j;
	t_vec2	p;

	p = pos;
	if ((editor->selected_tool == TOOL_OBJECTS || editor->selected_tool == TOOL_SELECT) && get_close_object(editor, &p))
		return (p);
	j = -1;
	while (++j < editor->rooms->len)
	{
		t_room	*room2 = &editor->rooms->values[j];
		t_bool	found = FALSE;
		p = point_on_room(editor, room2, pos, &found);
		if (found)
			return (p);
	}
	editor->grid_cell_grab = GG_NONE;
	if (!in_bounds((SDL_Rect){ 10, 70, S_WIDTH - 20, S_HEIGHT - 80 }, pos) 
		|| (is_settings_open(editor) && in_bounds((SDL_Rect){ S_WIDTH - 335, 75, 320, 550 }, pos)))
		editor->grid_cell_grab = GG_OUTSIDE;
	return (pos);
}
